#include <QtGui>

#include "VideoDecode.h"

#include <QDebug>

VideoDecode::VideoDecode(const QString &fileName)
{
	m_run = false;
	videoStream = -1;
	m_mediaValid = false;
	m_frameCounter = 0;

	avFormatCtx = NULL;
	avCodecCtx = NULL;
	swsCtx = NULL;

	frame = NULL;
	outFrame = NULL;
	frameRGB = NULL;

	outbuffer = NULL;

	initFFMPEG();
	setAVInput(fileName);
}

void VideoDecode::initFFMPEG()
{
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

	if (outbuffer) {
		av_free(outbuffer);
		outbuffer = NULL;
	}
}

void VideoDecode::setAVInput(const QString &fileName)
{
	if (isRunning()) {
		stop();
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
			videoStream = i;
			break;
		}
	}

	if (videoStream == -1) {
		qDebug() << __PRETTY_FUNCTION__ << "No Video Stream Found!";
		closeAVInput();
	        return;
	}

	int tb_num = avFormatCtx->streams[videoStream]->time_base.num;
	int tb_den = avFormatCtx->streams[videoStream]->time_base.den;
	int fr_num = avFormatCtx->streams[videoStream]->r_frame_rate.num;
	int fr_den = avFormatCtx->streams[videoStream]->r_frame_rate.den;

	qDebug() << "STREAM TIME BASE" << videoStream << "FPS:" << tb_num << "/" << tb_den;
	qDebug() << "STREAM R_FRAMERATE" << videoStream << "FPS:" << fr_num << "/" << fr_den;
	qDebug() << "CODEC"
		 << videoStream
		 << "FPS:"
		 << avFormatCtx->streams[videoStream]->codec->time_base.num
		 << "/"
		 << avFormatCtx->streams[videoStream]->codec->time_base.den;


	//double AvgTimePerFrame;
	if (tb_den != fr_num || tb_num != fr_den) {
		//AvgTimePerFrame = (((double)(fr_den)) / ((double)(fr_num)))*10000000;
		//qDebug() << "AVGTimePerFrame (Frate):" << AvgTimePerFrame << "ms";
		m_frameRate = (((double)(fr_den)) / ((double)(fr_num)))*1000000;//0;
		qDebug() << "AVGTimePerFrame (Frate):" << m_frameRate << "us";
	} else {
		//AvgTimePerFrame = (((double)(tb_num))/((double)(tb_den)))*10000000;
		//qDebug() << "AVGTimePerFrame (TimeBase):" << AvgTimePerFrame;
		m_frameRate = (((double)(tb_num))/((double)(tb_den)))*1000000;//0;
		qDebug() << "AVGTimePerFrame (TimeBase):" << m_frameRate << "us";
	}


	m_fps = av_q2d(avFormatCtx->streams[videoStream]->r_frame_rate);
	qDebug() << "fps:" << m_fps;
	//qDebug() << "timebase fps:" << av_q2d(avFormatCtx->streams[videoStream]->time_base);


	// Get a pointer to the codec context for the video stream
	avCodecCtx = avFormatCtx->streams[videoStream]->codec;

	// Find the decoder for the video stream
	pCodec = avcodec_find_decoder(avCodecCtx->codec_id);
	if (pCodec == NULL) {
		// Codec not found
		qDebug() << __PRETTY_FUNCTION__ << "Unsupported Video Codec!";
		closeAVInput();
		return;
	}

	// Open codec
	if (avcodec_open2(avCodecCtx, pCodec, NULL) < 0) {
		qDebug() << __PRETTY_FUNCTION__ << "Error Opening Codec!";
		closeAVInput();
		return;
	}

	// Allocate video frame
	frame    = avcodec_alloc_frame();
	outFrame = avcodec_alloc_frame();

	int w = avCodecCtx->width;
	int h = avCodecCtx->height;

	//qDebug() << w << "x" << h;


	// Calculate the bytes needed for the output image
	int nbytes = avpicture_get_size(AV_PIX_FMT_RGB24, w, h);
	//qDebug() <<  "nbytes:" << nbytes;

	// Create buffer for the output image
	outbuffer = (uint8_t *) av_malloc(nbytes);

	// This will set the pointers in the frame structures to the right points in
	// the input and output buffers.
	avpicture_fill((AVPicture*) outFrame, outbuffer, AV_PIX_FMT_RGB24, w, h);

	// Convert the image format (init the context the first time)
	swsCtx = sws_getCachedContext(swsCtx, w, h, avCodecCtx->pix_fmt, w, h, PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);
	if (swsCtx == NULL) {
		qDebug() << __PRETTY_FUNCTION__ << "Error initializing Conversion Context!";
		closeAVInput();
		return;
	}

	LastFrame = QImage(w, h, QImage::Format_RGB888);
	m_mediaValid = true;
}

bool VideoDecode::mediaValid() const
{
	return m_mediaValid;
}

QImage VideoDecode::lastFrame()
{
	return LastFrame;
}

QSize VideoDecode::videoSize() const
{
	if (!m_mediaValid) {
		return QSize(0,0);
	}
	return QSize(avCodecCtx->width, avCodecCtx->height);
}

int VideoDecode::videoLengthMs() const
{
	if (!m_mediaValid) {
		return -1;
	}

	int secs = avFormatCtx->duration / AV_TIME_BASE;
	int us   = avFormatCtx->duration % AV_TIME_BASE;
	int ms   = secs*1000 + us/1000;

	qDebug() << __PRETTY_FUNCTION__ << ms << "ms";
	//qDebug() << avFormatCtx->nb_index_entries;
	return ms;
}


void VideoDecode::stop()
{
	m_run = false;
}

void VideoDecode::run()
{
	if (!m_mediaValid) {
		return;
	}

	m_run = true;
	int frameFinished;
	int w = avCodecCtx->width;
	int h = avCodecCtx->height;

	while ((m_run) && (av_read_frame(avFormatCtx, &packet) >= 0)) {
		// Is this a packet from the video stream?
		if (packet.stream_index == videoStream) {
		        // Decode video frame
			avcodec_decode_video2(avCodecCtx, frame, &frameFinished, &packet);
			// Did we get a video frame?
			if (frameFinished) {
				// Convert the image from its native format to RGB
				sws_scale(swsCtx, frame->data, frame->linesize, 0, h /*avCodecCtx->height*/, outFrame->data, outFrame->linesize);

				// Convert the frame to QImage
				for (int y = 0; y < h; y++) {
					memcpy(LastFrame.scanLine(y), outFrame->data[0] + y*outFrame->linesize[0], w*3);
				}
#if 0
				QString s = QString("frame%1.png").arg(m_frameCounter);
				qDebug() << s;
				if (m_frameCounter == 10) {
					LastFrame.save(s,"PNG");
				}
#endif

				emit frameReady();
				//qDebug() << "Frame:" << m_frameCounter;
				m_frameCounter++;
				this->usleep((unsigned long) m_frameRate);
			}
		}

		// Free the packet that was allocated by av_read_frame
		av_free_packet(&packet);
	}
}
