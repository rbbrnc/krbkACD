#include "VideoWidget.h"

#include <QLabel>
#include <QPixmap>

#include <QDebug>

VideoWidget::VideoWidget(QWidget *parent)
	: QWidget(parent)
{
	QVBoxLayout *layout = new QVBoxLayout;
	label = new QLabel(this);
	label->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);


	int w = this->width();
	int h = this->height();

	resize(w, h);

	layout->addWidget(label);
	setLayout(layout);
}

VideoWidget::VideoWidget(const QString &fileName, QWidget *parent)
	: QWidget(parent)
{
	QVBoxLayout *layout = new QVBoxLayout;

	label = new QLabel(this);
	label->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);


	m_avThread = new VideoDecode(fileName);

	if (m_avThread->mediaValid()) {
		label->resize(m_avThread->videoSize());
	} else {
		label->resize(320, 240);
	}

//	updateLabel();

	QPushButton *play = new QPushButton("Play");
	connect(play, SIGNAL(clicked()), this, SLOT(playVideo()));


	layout->addWidget(label);
	layout->addWidget(play);
	setLayout(layout);

	connect(m_avThread, SIGNAL(frameReady()), this, SLOT(updateLabel()));

	m_avThread->videoLengthMs();
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

void VideoWidget::playVideo()
{
	if (m_avThread->isRunning()) {
		qDebug() << __PRETTY_FUNCTION__ << "STOP";
		m_avThread->stop();
	} else {
		qDebug() << __PRETTY_FUNCTION__ << "PLAY";
		m_avThread->start();
	}
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

