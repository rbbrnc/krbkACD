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
		void init();
		void closeAVInput();

		void seekVideoFrame();
		void decodeVideoFrame();

	public slots:
		void seekRequest(double seconds);
		void save(const QString &fname);

	signals:
		void frameReady();
		void frameReady(qint64 frameTimestamp);

	private:
		int  m_videoStream;	// Video stream index
		bool m_mediaValid;
		bool m_run;

		// Frame dimensions (w, h)
		int m_width;
		int m_height;

		AVFormatContext *avFormatCtx;
		AVCodecContext  *avCodecCtx;
		SwsContext      *m_swsCtx;

		AVCodec *m_videoCodec;

		AVPacket m_packet;

		AVFrame *m_frame;
		AVFrame *m_outFrame;
		uint8_t *m_outFrameBuffer;

		QImage m_image;		// Current frame picture

		double  m_fps;		// Frame Per Seconds
		double  m_frameRate;	// micro seconds
		int64_t m_frameCounter;
		int64_t m_frameCurrentTime;

		bool    m_seekRequest;
		int     m_seekFlags;	// ffmpeg seek flags
		int64_t m_seekTarget;	// seek target position
};

#endif
