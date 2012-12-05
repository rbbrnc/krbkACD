#include "AvManager.h"
#include "ui_AvManager.h"

#include <QTime>
#include <QDebug>

AvManager::AvManager(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::AvManager)
{
	ui->setupUi(this);
}

AvManager::~AvManager()
{
	m_avThread->stop();
	delete ui;
}

AvManager::AvManager(const QString &fileName, QWidget *parent)
	: QWidget(parent),
	ui(new Ui::AvManager)
{
	ui->setupUi(this);
	//label->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

	m_avThread = new VideoDecode(fileName);

	if (m_avThread->mediaValid()) {
		ui->label->resize(m_avThread->videoSize());
	} else {
		ui->label->resize(320, 240);
	}

	connect(ui->playButton, SIGNAL(clicked()), this, SLOT(playVideo()));
	connect(ui->ffwdButton, SIGNAL(clicked()), this, SLOT(ffwdVideo()));
	connect(ui->rewButton, SIGNAL(clicked()), this, SLOT(rewVideo()));
	connect(m_avThread, SIGNAL(frameReady(qint64)), this, SLOT(updateFrame(qint64)));
	connect(ui->seekSlider, SIGNAL(sliderReleased()), this, SLOT(seekFrame()));

	// duration in microseconds.
	m_duration = m_avThread->videoLengthMs() * 1000;

	QTime tm;
	tm = tm.addMSecs(m_avThread->videoLengthMs());
	qDebug() << "Duration:" << tm.toString("HH:mm:ss:zzz");
}

void AvManager::updateTimeLabel(qint64 time)
{
	QTime tm;
	tm = tm.addMSecs(time / 1000);
	ui->currentTime->setText(tm.toString("HH:mm:ss:zzz"));
}

void AvManager::updateFrame(qint64 time)
{
	QImage img = m_avThread->lastFrame();
	ui->label->setPixmap(QPixmap::fromImage(img));

	updateTimeLabel(time);

	qint64 pos = (time * 100) / m_duration;
	//qDebug() << __PRETTY_FUNCTION__ << time << m_duration << pos << "%";

	ui->seekSlider->setSliderPosition(pos);
}

void AvManager::ffwdVideo()
{
	// seek forward of 10 secs.
	m_avThread->seekRequest(10.0);
}

void AvManager::rewVideo()
{
	// seek backward of 10 secs.
	m_avThread->seekRequest(-10.0);
}

void AvManager::playVideo()
{
	if (m_avThread->isRunning()) {
		m_avThread->stop();
		ui->playButton->setText("PLAY");
	} else {
		m_avThread->start();
		ui->playButton->setText("STOP");
	}
}

void AvManager::seekFrame()
{
	if (m_avThread->isRunning()) {
		return;
	}

	// before seek at least 1 frame must be readed.
//	bool b = m_avThread->readVideoFrame();

	qint64 pos = m_duration * ui->seekSlider->value() / 100;
	m_avThread->seekToAbsoluteTime(pos);

	bool b = false;
	while (!b) {
		b = m_avThread->readVideoFrame();
	}

	qDebug() << __PRETTY_FUNCTION__ << b;
	updateTimeLabel(pos);
}

