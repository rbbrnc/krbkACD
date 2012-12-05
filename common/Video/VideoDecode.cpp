#include <QtGui>

#include "VideoDecode.h"

#include <QDebug>

#include <limits>

VideoDecode::VideoDecode()
{
	init();
}

VideoDecode::VideoDecode(const QString &fileName)
{
	init();
	setAVInput(fileName);
}

void VideoDecode::init()
{
	m_run = false;
	m_seekRequest = false;

	m_videoStreamIndex = -1;
	m_mediaValid = false;

	avFormatCtx = NULL;
	avCodecCtx = NULL;

	// SWS Context
	m_swsCtx = NULL;

	m_frame = NULL;
	m_outFrame = NULL;
	m_outFrameBuffer = NULL;

	// Register all the codecs, parsers and bitstream filters
	// which were enabled at configuration time.
	avcodec_register_all();

	// Initialize libavformat and register all muxers, demuxers and protocols.
	av_register_all();
}

void VideoDecode::closeAVInput()
{
	if (isRunning()) {
		stop();
	}

	m_mediaValid = false;
	if (avFormatCtx) {
		avformat_close_input(&avFormatCtx);
		avFormatCtx = NULL;
	}

	if (m_outFrameBuffer) {
		av_free(m_outFrameBuffer);
		m_outFrameBuffer = NULL;
	}
}


void VideoDecode::fpsEstimation()
{
	// Stream timebase
	int tb_num = m_videoStream->time_base.num;
	int tb_den = m_videoStream->time_base.den;

	// Stream frame rate
	int fr_num = m_videoStream->r_frame_rate.num;
	int fr_den = m_videoStream->r_frame_rate.den;

	// Codec timebase
	int ctb_num = avCodecCtx->time_base.num;
	int ctb_den = avCodecCtx->time_base.den;


	if (m_videoStream->nb_frames > 0) {
		qDebug() << "[Stream] - Duration (frames):" << m_videoStream->nb_frames;
	} else {
		qDebug() << "[Stream] - Duration (frames): Unavailable.";
	}
	qDebug() << "[Stream] - PTS wrap bits:" << m_videoStream->pts_wrap_bits;

	double fAverageTimeStampsPerSeconds = (double) m_videoStream->time_base.den / (double) m_videoStream->time_base.num;
	qDebug() << "[Stream] - TimeBase:" << m_videoStream->time_base.num << ":" << m_videoStream->time_base.den;
	qDebug() << "[Stream] - Average timestamps per seconds:" << fAverageTimeStampsPerSeconds;

	double fAvgFrameRate = 0.0;
	if (m_videoStream->avg_frame_rate.den != 0) {
		fAvgFrameRate = av_q2d(m_videoStream->avg_frame_rate);
		qDebug() << "[Stream] - Average FPS:" << fAvgFrameRate;
	} else {
		qDebug() << "[Stream] - Average FPS: Unavailable.";
	}

	int64_t iFirstTimeStamp = (int64_t)((double)((double) avFormatCtx->start_time / (double) AV_TIME_BASE) * fAverageTimeStampsPerSeconds);
	if (avFormatCtx->start_time > 0)
		iFirstTimeStamp = (int64_t)((double)((double) avFormatCtx->start_time / (double) AV_TIME_BASE) * fAverageTimeStampsPerSeconds);
	else
		iFirstTimeStamp = 0;

	qDebug() << "[Container] - Start time (us): " << avFormatCtx->start_time;
	qDebug() << "[Container] - Start timestamp: " << iFirstTimeStamp;

	qDebug() << "[Codec] - Name:" << avCodecCtx->codec_name << "id:" << avCodecCtx->codec_id;
	qDebug() << "[Codec] - TimeBase:" << avCodecCtx->time_base.den << ":" << avCodecCtx->time_base.num;

	int iTicksPerFrame = avCodecCtx->ticks_per_frame;
	qDebug() << "[Codec] - Ticks per frame:" << iTicksPerFrame;
	qDebug() << "[Codec] - Bit rate:" << avCodecCtx->bit_rate;

	// Compute total duration in TimeStamps.
	int64_t iDurationTimeStamps;
	if (avFormatCtx->duration > 0) {
		// av_rescale ?
		iDurationTimeStamps = (int64_t)((double)((double) avFormatCtx->duration / (double) AV_TIME_BASE) * fAverageTimeStampsPerSeconds);
	} else {
		// todo : try SuperSeek technique. Seek @ +10 Hours, to get the last I-Frame
		iDurationTimeStamps = 0;
	}
	qDebug() << "Duration in timestamps:" << iDurationTimeStamps;
	qDebug() << "Duration in seconds (computed):" << (double)((double) iDurationTimeStamps/(double) fAverageTimeStampsPerSeconds);

	//----------------------------------------------------------------------------------------------------------------------
	// FPS Moyen.
	// Sur un Play, la cadence des frames ne reflètera pas forcément la vraie cadence si le fichier à un framerate variable.
	// On considère que c'est un cas rare et que la différence ne va pas trop géner.
	//
	// Quatre sources pour calculer le FPS moyen, à tester dans l'ordre :
	//
	//  - le calcul fait par libav dans le stream.
	//  - les infos de duration en frames et en µs, du conteneur et du stream. (Rarement disponibles, mais valide si oui)
	//	- le Stream->time_base	(Souvent ko, sous la forme de 90000:1, sert en fait à exprimer l'unité des timestamps)
	//  - le Codec->time_base	(Souvent ok, mais pas toujours.)
	//
	//----------------------------------------------------------------------------------------------------------------------
	double fFps = 0;
	bool bFpsIsReliable = true;

	// Did libav provide an estimate ?
	if(fAvgFrameRate != 0) {
		fFps = fAvgFrameRate;
		qDebug() << "Average Fps estimation method: libav.";
	} else {
		// 1.a. Par les durations
		if ((m_videoStream->nb_frames > 0) && (avFormatCtx->duration > 0)) {
			fFps = ((double) m_videoStream->nb_frames * (double) AV_TIME_BASE) / (double) avFormatCtx->duration;
			if (iTicksPerFrame > 1) {
				fFps /= iTicksPerFrame;
			}
			qDebug() << "Average Fps estimation method: Durations.";
		} else {
			// 1.b. Par le Stream->time_base, on invalide si >= 1000.
			fFps = (double) m_videoStream->time_base.den / (double) m_videoStream->time_base.num;
			if (fFps < 1000) {
				if(iTicksPerFrame > 1) {
					fFps /= iTicksPerFrame;
				}
				qDebug() << "Average Fps estimation method: Stream timebase.";
			} else {
				// 1.c. Par le Codec->time_base, on invalide si >= 1000.
				fFps = (double) avCodecCtx->time_base.den / (double) avCodecCtx->time_base.num;
				if (fFps < 1000) {
					if(iTicksPerFrame > 1) {
						fFps /= iTicksPerFrame;
					}
					qDebug() << "Average Fps estimation method: Codec timebase.";
				} else if (fFps == 30000) {
						fFps = 29.97;
						qDebug() << "Average Fps estimation method: special case detection (30000:1 -> 30000:1001).";
				} else if (fFps == 25000) {
						fFps = 24.975;
						qDebug() << "Average Fps estimation method: special case detection (25000:1 -> 25000:1001).";
				} else {
					//---------------------------------------------------------------------------
					// Le fichier ne nous donne pas assez d'infos, ou le frame rate est variable.
					// Forcer à 25 fps.
					//---------------------------------------------------------------------------
					fFps = 25;
					bFpsIsReliable = false;
					qDebug() << "Average Fps estimation method: Estimation failed. Fps will be forced to :" << fFps;
				}
			}
		}
	}

	qDebug() << "Average Fps:" << fFps;

	double fFrameInterval = (double)1000/fFps;
	qDebug() << "Average Frame Interval (ms):" << fFrameInterval;

	// Precomputations.
	int64_t iAverageTimeStampsPerFrame = (int64_t) round(fAverageTimeStampsPerSeconds / fFps);
	qDebug() << "Average Timestamps per frame:" << iAverageTimeStampsPerFrame;

	// Calc. FPS
	// Real base framerate of the stream
	m_fps = av_q2d(m_videoStream->r_frame_rate);

	if (tb_den != fr_num || tb_num != fr_den) {
		m_frameRate = 1 / m_fps * AV_TIME_BASE;
		qDebug() << "AVGTimePerFrame (Frate):" << m_frameRate << "us";
	} else {
		m_frameRate = av_q2d(m_videoStream->time_base) * AV_TIME_BASE;
		qDebug() << "AVGTimePerFrame (TimeBase):" << m_frameRate << "us";
	}

	qDebug() << "FPS:" << m_fps;
	qDebug() << "STREAM TIME BASE (FPS)"   << tb_num << "/" << tb_den;
	qDebug() << "STREAM R_FRAMERATE (FPS)" << fr_num << "/" << fr_den;
	qDebug() << "CODEC (FPS)"              << ctb_num << "/" << ctb_den;
}

void VideoDecode::setAVInput(const QString &fileName)
{
	if (isRunning()) {
		stop();
	}

	if (m_mediaValid) {
		closeAVInput();
	}

	int err = 0;

	// Open video file
	err = avformat_open_input(&avFormatCtx, (const char *)(QFile::encodeName(fileName)), NULL, NULL);
	if (err) {
		qDebug() << __PRETTY_FUNCTION__ << "ERROR Opening:" << fileName;
		return;
	}

	if (avformat_find_stream_info(avFormatCtx, 0) < 0) {
		qDebug() << __PRETTY_FUNCTION__ << "No Stream Info Found!";
		closeAVInput();
		return;
	}

	// Print format info
	av_dump_format(avFormatCtx, 0, (const char *)(QFile::encodeName(fileName)), 0);

	// Find the first video stream
	for (unsigned int i = 0; i < avFormatCtx->nb_streams; i++) {
		if (avFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
			m_videoStreamIndex = i;
			break;
		}
	}

	if (m_videoStreamIndex == -1) {
		qDebug() << __PRETTY_FUNCTION__ << "No Video Stream Found!";
		closeAVInput();
	        return;
	}

	// Save the pointer to the 1st video stream
	m_videoStream = avFormatCtx->streams[m_videoStreamIndex];

	// Get a pointer to the codec context for the video stream
	avCodecCtx = m_videoStream->codec;

	// Find the decoder for the video stream
	m_videoCodec = avcodec_find_decoder(avCodecCtx->codec_id);
	if (m_videoCodec == NULL) {
		// Codec not found
		qDebug() << __PRETTY_FUNCTION__ << "Unsupported Video Codec!";
		closeAVInput();
		return;
	}

	// Open codec
	if (avcodec_open2(avCodecCtx, m_videoCodec, NULL) < 0) {
		qDebug() << __PRETTY_FUNCTION__ << "Error Opening Codec!";
		closeAVInput();
		return;
	}

	fpsEstimation();
///7
	// Other datas.
	qDebug() << "[Codec] - Has B Frames:" << avCodecCtx->has_b_frames;


	m_width  = avCodecCtx->width;
	m_height = avCodecCtx->height;
	qDebug() << "[Codec] - Width (pixels):"  << m_width;
	qDebug() << "[Codec] - Height (pixels):" << m_height;

	qDebug() << "--------------------------------------";

	// Allocate video frames
	m_frame    = avcodec_alloc_frame();
	m_outFrame = avcodec_alloc_frame();

	// Calculate the bytes needed for the output image
	int nbytes = avpicture_get_size(AV_PIX_FMT_RGB24, m_width, m_height);
	//qDebug() <<  "nbytes:" << nbytes;

	// Create buffer for the output image
	m_outFrameBuffer = (uint8_t *) av_malloc(nbytes);

	// This will set the pointers in the frame structures to the right points in
	// the input and output buffers.
	avpicture_fill((AVPicture*) m_outFrame, m_outFrameBuffer, AV_PIX_FMT_RGB24, m_width, m_height);

	// Convert the image format (init the context the first time)
	m_swsCtx = sws_getCachedContext(m_swsCtx, m_width, m_height,
				      avCodecCtx->pix_fmt, m_width, m_height,
				      PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);
	if (m_swsCtx == NULL) {
		qDebug() << __PRETTY_FUNCTION__ << "Error initializing Conversion Context!";
		closeAVInput();
		return;
	}

	m_image = QImage(m_width, m_height, QImage::Format_RGB888);

	m_mediaValid = true;
}

bool VideoDecode::mediaValid() const
{
	return m_mediaValid;
}

QImage VideoDecode::lastFrame()
{
	return m_image;
}

QSize VideoDecode::videoSize() const
{
	if (!m_mediaValid) {
		return QSize(0,0);
	}
	return QSize(avCodecCtx->width, avCodecCtx->height);
}

/*
 * Get stream duration in milliseconds
 *
 * seconds      = avFormatCtx->duration / AV_TIME_BASE;
 * microseconds = avFormatCtx->duration % AV_TIME_BASE;
 * milliseconds = seconds*1000 + microseconds/1000;
 *
 */
qint64 VideoDecode::videoLengthMs() const
{
	if (!m_mediaValid) {
		return -1;
	}

	return  (avFormatCtx->duration / AV_TIME_BASE) * 1000 +
		(avFormatCtx->duration % AV_TIME_BASE) / 1000;
}

// [SLOT public]
void VideoDecode::seekToAbsoluteTime(qint64 microseconds)
{
	if (!m_seekRequest) {
		m_seekFlags &= ~AVSEEK_FLAG_BYTE; /* seek by time */
		m_seekTarget = microseconds;
		m_seekRequest = true;
		qDebug() << __PRETTY_FUNCTION__ << "Seek to:" << microseconds;
	}
}

void VideoDecode::seekRequest(double seconds)
{
	if (!m_seekRequest) {
		m_seekFlags &= ~AVSEEK_FLAG_BYTE; /* seek by time */
		m_seekTarget = m_frameCurrentTime + (int64_t)(seconds * AV_TIME_BASE);
		m_seekRequest = true;

		qDebug() << __PRETTY_FUNCTION__ << "Sec:" << seconds
			 << "FROM" << m_frameCurrentTime
			 << "DELTA" << (int64_t)(seconds * AV_TIME_BASE)
			 << "TO" << m_seekTarget;
	}
}

void VideoDecode::seekVideoFrame()
{
	if (m_videoStreamIndex < 0) {
		return;
	}

	int64_t seekMin = std::numeric_limits<int64_t>::min(); /* INT64_MIN */
	int64_t seekMax = std::numeric_limits<int64_t>::max(); /* INT64_MAX */
	int rc;

	rc = avformat_seek_file(avFormatCtx, -1, seekMin, m_seekTarget, seekMax, m_seekFlags);
	if (rc < 0) {
		qDebug() << "Error while seeking [rc]:" << rc;
	} else {
		avcodec_flush_buffers(avCodecCtx);
	}
}

void VideoDecode::dumpVideoPacket(int frameFinished) const
{
	QTime tm;
	tm = tm.addMSecs(m_frameCurrentTime / 1000);
	qDebug() << "Frame K:" << m_frame->key_frame << "Decoded:" << frameFinished
		 << "Frame BETs:" << av_frame_get_best_effort_timestamp(m_frame)
		 << "Frame Time:" << m_frameCurrentTime << "us" << tm.toString("HH:mm:ss:zzz")
		 << "Packet PTS [tbu]:" << m_packet.pts	// Presentation timestamp in AVStream->time_base units; the time at which the decompressed packet will be presented to the user.
		 << "Packet DTS [tbu]:" << m_packet.dts	// Decompression timestamp in AVStream->time_base units; the time at which the packet is decompressed.
		 << "Packet Duration [tbu]:" << m_packet.duration	// duration Duration of this packet in AVStream->time_base units, 0 if unknown.
		 << "Packet Pos [byte]:" << m_packet.pos	//int64_t byte position in stream, -1 if unknown
//		 << "Packet flags:" << m_packet.flags
		 << "";

//int64_t convergence_duration
// Time difference in AVStream->time_base units from the pts of this packet to the point at which the output from the decoder has converged
// independent from the availability of previous frames.
}

bool VideoDecode::decodeVideoFrame()
{
	int frameFinished;

	// Is this a packet from the video stream?
	if (m_packet.stream_index == m_videoStreamIndex) {
	        // Decode video frame
		int len = avcodec_decode_video2(avCodecCtx, m_frame, &frameFinished, &m_packet);
		// Did we get a video frame?
		if ((len > 0) && frameFinished) {
			//int64_t pts = av_frame_get_best_effort_timestamp(m_frame);
			//int tb_num = m_videoStream->time_base.num;
			//int tb_den = m_videoStream->time_base.den;
			//int64_t timestamp = AV_TIME_BASE * pts * tb_num / tb_den;
#if 10
			m_frameCurrentTime = av_frame_get_best_effort_timestamp(m_frame);
			if (m_frameCurrentTime == AV_NOPTS_VALUE) {
				qDebug() << __PRETTY_FUNCTION__ << "no BET";
				m_frameCurrentTime = m_videoStream->pts.val;
				if (m_frameCurrentTime == AV_NOPTS_VALUE) {
					qDebug() << __PRETTY_FUNCTION__ << "no PTS";
					m_frameCurrentTime = m_packet.dts;
				}
			}
			m_frameCurrentTime *= m_frameRate;
#else
			m_frameCurrentTime = m_frameRate * av_frame_get_best_effort_timestamp(m_frame);
#endif

			// Convert the image from its native format to RGB
			sws_scale(m_swsCtx, m_frame->data, m_frame->linesize, 0, avCodecCtx->height, m_outFrame->data, m_outFrame->linesize);

			// Convert the frame to QImage
			for (int y = 0; y < m_height; y++) {
				memcpy(m_image.scanLine(y),
				       m_outFrame->data[0] + y*m_outFrame->linesize[0],
				       m_width * 3);
			}

			//qDebug() << __PRETTY_FUNCTION__ << "no video stream";
			emit frameReady(m_frameCurrentTime);

			dumpVideoPacket(frameFinished);

			this->usleep((unsigned long) m_frameRate);
			return true;
		} else {
			dumpVideoPacket(frameFinished);
			return false;
		}
	} else {
		//qDebug() << __PRETTY_FUNCTION__ << "no video stream";
		return false;
	}
}

bool VideoDecode::readVideoFrame()
{
	if (m_seekRequest) {
		seekVideoFrame();
		m_seekRequest = false;
	}

	int rc = av_read_frame(avFormatCtx, &m_packet);
	if (rc >= 0) {
		bool b = decodeVideoFrame();
		// Free the packet that was allocated by av_read_frame
		av_free_packet(&m_packet);
		return b;
	} else {
		qDebug() << "av_read_frame:" << av_err2str(rc);
		return false;
	}
}

void VideoDecode::run()
{
	if (!m_mediaValid) {
		return;
	}

	m_run = true;

	int rc;

	while (m_run) {
#if 10
		if (m_seekRequest) {
			seekVideoFrame();
			m_seekRequest = false;
		}
		rc = av_read_frame(avFormatCtx, &m_packet);
		if (rc >= 0) {
			decodeVideoFrame();
			// Free the packet that was allocated by av_read_frame
			av_free_packet(&m_packet);
		} else {
			qDebug() << "av_read_frame:" << av_err2str(rc);
			stop();
		}
#endif
	}
}

void VideoDecode::stop()
{
	m_run = false;
}

// [SLOT public]
//
// Save last frame picture to .png file
//
void VideoDecode::save(const QString &fname)
{
	if (!m_image.isNull()) {
		m_image.save(fname, "PNG");
	}
}
