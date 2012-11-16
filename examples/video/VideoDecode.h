#ifndef VIDEO_DECODE_H
#define VIDEO_DECODE_H

#include <QThread>
#include <QImage>

/* Fix:
 * libavutil/common.h:154: error: UINT64_C was not declared in this scope
 */
#ifdef __cplusplus
#define __STDC_CONSTANT_MACROS
#ifdef _STDINT_H
#undef _STDINT_H
#endif
#include <stdint.h>
#endif
/* Fix End. */

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

class VideoDecode : public QThread
{
	Q_OBJECT

	public:
		VideoDecode(const QString &fileName);

		void setAVInput(const QString &fileName);

		QImage lastFrame();
		QSize  videoSize() const;
		bool   mediaValid() const;
		int    videoLengthMs() const;

		void run();
		void stop(void);

	private:
		void initFFMPEG();
		void closeAVInput();

		void seekVideoFrame();
		void decodeVideoFrame();

	public slots:
		void seekRequest(double seconds);

	signals:
		void frameReady();

	private:
		int videoStream;
		bool m_mediaValid;
		bool m_run;

		// Frame dimensions (w, h)
		int m_width;
		int m_height;

		AVFrame *outFrame;
		AVFrame *frameRGB;
		AVCodec *pCodec;

		AVFormatContext *avFormatCtx;
		AVCodecContext  *avCodecCtx;
		AVFrame *frame;

		uint8_t *outbuffer;

		SwsContext *swsCtx;

		AVPacket m_packet;

		QImage LastFrame;

		double m_fps;
		double m_frameRate; // micro seconds

		unsigned int m_frameCounter;

		int64_t DesiredFrameNumber;

		bool    m_seekRequest;
		int     m_seekFlags;
		int64_t m_seekPos;

		double m_timePos; // seconds

		int64_t m_frameCurrentTime;
		int64_t m_frameStartTime;
};

#endif
