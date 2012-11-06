#include <QtGui>

#include "VideoDecode.h"

#include <QDebug>

VideoDecode::VideoDecode(const QString &fileName)
{
	videoStream = -1;

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
			videoStream = i;
			break;
		}
	}

	if (videoStream == -1) {
		qDebug() << __PRETTY_FUNCTION__ << "No Video Stream Found!";
		closeAVInput();
	        return;
	}

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

	qDebug() << w << "x" << h;


	// Calculate the bytes needed for the output image
	int nbytes = avpicture_get_size(AV_PIX_FMT_RGB24, w, h);
	qDebug() <<  "nbytes:" << nbytes;

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
}

QImage VideoDecode::lastFrame()
{
	return LastFrame;
}

void VideoDecode::stop()
{
	//m_sc_run = false;
}

void VideoDecode::run()
{
//	m_plc.init = true;
//	m_sc_run = true;

//	while (m_sc_run) {
//		updatePLCPoints();
//		emit plcUpdate(m_plc);
//	}

	int frameFinished;

	unsigned int count = 0;
	int w = avCodecCtx->width;
	int h = avCodecCtx->height;

	while (av_read_frame(avFormatCtx, &packet) >= 0) {
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
				QString s = QString("frame%1.png").arg(count);
				qDebug() << s;
				if (count == 10) {
					LastFrame.save(s,"PNG");
				}
#endif

				emit frameReady();
				count++;
				this->msleep(200);
			}
		}

		// Free the packet that was allocated by av_read_frame
		av_free_packet(&packet);
	}
}
