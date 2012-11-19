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

	m_videoStream = -1;
	m_mediaValid = false;
	m_frameCounter = 0;

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

void VideoDecode::setAVInput(const QString &fileName)
{
	if (isRunning()) {
		stop();
	}

	if (m_mediaValid) {
		closeAVInput();
	}

	int err = 0;
	m_frameCounter = 0;

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
			m_videoStream = i;
			break;
		}
	}

	if (m_videoStream == -1) {
		qDebug() << __PRETTY_FUNCTION__ << "No Video Stream Found!";
		closeAVInput();
	        return;
	}

	// Stream timebase
	int tb_num = avFormatCtx->streams[m_videoStream]->time_base.num;
	int tb_den = avFormatCtx->streams[m_videoStream]->time_base.den;

	// Stream frame rate
	int fr_num = avFormatCtx->streams[m_videoStream]->r_frame_rate.num;
	int fr_den = avFormatCtx->streams[m_videoStream]->r_frame_rate.den;

	// Codec timebase
	int ctb_num = avFormatCtx->streams[m_videoStream]->codec->time_base.num;
	int ctb_den = avFormatCtx->streams[m_videoStream]->codec->time_base.den;

	// Calc. FPS
	m_fps = av_q2d(avFormatCtx->streams[m_videoStream]->r_frame_rate);

	if (tb_den != fr_num || tb_num != fr_den) {
		m_frameRate = 1 / m_fps * AV_TIME_BASE;
		qDebug() << "AVGTimePerFrame (Frate):" << m_frameRate << "us";
	} else {
		m_frameRate = av_q2d(avFormatCtx->streams[m_videoStream]->time_base);
		m_frameRate *= AV_TIME_BASE;
		qDebug() << "AVGTimePerFrame (TimeBase):" << m_frameRate << "us";
	}

	qDebug() << "FPS:" << m_fps;
	qDebug() << "STREAM TIME BASE (FPS)"   << tb_num << "/" << tb_den;
	qDebug() << "STREAM R_FRAMERATE (FPS)" << fr_num << "/" << fr_den;
	qDebug() << "CODEC (FPS)"              << ctb_num << "/" << ctb_den;


	// Get a pointer to the codec context for the video stream
	avCodecCtx = avFormatCtx->streams[m_videoStream]->codec;

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

	// Allocate video frames
	m_frame    = avcodec_alloc_frame();
	m_outFrame = avcodec_alloc_frame();

	m_width  = avCodecCtx->width;
	m_height = avCodecCtx->height;

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
	int64_t seekMin = std::numeric_limits<int64_t>::min(); /* INT64_MIN */
	int64_t seekMax = std::numeric_limits<int64_t>::max(); /* INT64_MAX */
	int rc;

	rc = avformat_seek_file(avFormatCtx, -1, seekMin, m_seekTarget, seekMax, m_seekFlags);
	if (rc < 0) {
		qDebug() << "Error while seeking";
	} else {
		if (m_videoStream >= 0) {
			avcodec_flush_buffers(avCodecCtx);
		}
	}
}

void VideoDecode::decodeVideoFrame()
{
	int frameFinished;

	// Is this a packet from the video stream?
	if (m_packet.stream_index == m_videoStream) {
	        // Decode video frame
		int len = avcodec_decode_video2(avCodecCtx, m_frame, &frameFinished, &m_packet);
		// Did we get a video frame?
		if ((len > 0) && frameFinished) {
			//int64_t pts = av_frame_get_best_effort_timestamp(m_frame);
			//int tb_num = avFormatCtx->streams[m_videoStream]->time_base.num;
			//int tb_den = avFormatCtx->streams[m_videoStream]->time_base.den;
			//int64_t timestamp = AV_TIME_BASE * pts * tb_num / tb_den;
			m_frameCurrentTime = m_frameRate * av_frame_get_best_effort_timestamp(m_frame);

			// Convert the image from its native format to RGB
			sws_scale(m_swsCtx, m_frame->data, m_frame->linesize, 0, avCodecCtx->height, m_outFrame->data, m_outFrame->linesize);

			// Convert the frame to QImage
			for (int y = 0; y < m_height; y++) {
				memcpy(m_image.scanLine(y),
				       m_outFrame->data[0] + y*m_outFrame->linesize[0],
				       m_width * 3);
			}

			emit frameReady(m_frameCurrentTime);

#if 0
			// DEBUG
			QTime tm;
			tm = tm.addMSecs(m_frameCurrentTime / 1000);
			qDebug() << "Frame" << m_frameCounter << "K:" << m_frame->key_frame
				 //<< "PTS:" << m_packet.pts
				 //<< "DTS:" << m_packet.dts
				 //<< "Dur:" << m_packet.duration
				 //<< "BETs:" << av_frame_get_best_effort_timestamp(m_frame)
				 << "Time:" << m_frameCurrentTime << "us" << tm.toString("HH:mm:ss:zzz");
#endif
			m_frameCounter++;
			this->usleep((unsigned long) m_frameRate);
		}
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
		if (m_seekRequest) {
			seekVideoFrame();
			qDebug() << "SR";
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
