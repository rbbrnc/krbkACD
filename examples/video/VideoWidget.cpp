#include "VideoWidget.h"

#include <QLabel>
#include <QPixmap>

#include <QDebug>

VideoWidget::VideoWidget(QWidget *parent)
	: QWidget(parent)
{
	label = new QLabel(this);
	label->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

	int w = this->width();
	int h = this->height();

	resize(w, h);
}

VideoWidget::VideoWidget(const QString &fileName, QWidget *parent)
	: QWidget(parent)
{
	label = new QLabel(this);
	label->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);


	m_avThread = new VideoDecode(fileName);

	if (m_avThread->mediaValid()) {
		resize(m_avThread->videoSize());
	} else {
		resize(320, 240);
	}

	updateLabel();

	connect(m_avThread, SIGNAL(frameReady()), this, SLOT(updateLabel()));

	m_avThread->videoLengthMs();

	m_avThread->start();
}


VideoWidget::~VideoWidget()
{
	m_avThread->stop();
}

void VideoWidget::updateLabel()
{
	QImage img = m_avThread->lastFrame();
	label->setPixmap(QPixmap::fromImage(img));
}

void VideoWidget::resizeEvent(QResizeEvent *event)
{
	int w = event->size().width();
	int h = event->size().height();
#if 0
	if (!m_pic.isNull()) {
		m_pic = m_pic.scaled(w, h, Qt::KeepAspectRatio);
		label->setPixmap(m_pic);
	}

	for (int i = 0; i < m_tagWidgetList.size(); i++) {
		m_tagWidgetList.at(i)->setFrameSize(w, h);
	 }
#endif
}

