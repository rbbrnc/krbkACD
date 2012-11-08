#ifndef VIDEO_WIDGET_H
#define VIDEO_WIDGET_H

#include <QtGui>

#include "VideoDecode.h"

class VideoWidget : public QWidget
{
	Q_OBJECT

	public:
		VideoWidget(QWidget *parent = 0);
		VideoWidget(const QString &fileName, QWidget *parent = 0);
		~VideoWidget();

	private slots:
		void updateLabel();
		void playVideo();

	protected:
		void resizeEvent(QResizeEvent *event);


	private:
		QLabel *label;
		QPixmap m_pic;

		VideoDecode *m_avThread;
};

#endif
