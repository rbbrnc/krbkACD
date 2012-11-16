#include "AvManager.h"
#include "ui_AvManager.h"

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
	connect(m_avThread, SIGNAL(frameReady()), this, SLOT(updateLabel()));

	m_avThread->videoLengthMs();
}

void AvManager::updateLabel()
{
	QImage img = m_avThread->lastFrame();
	ui->label->setPixmap(QPixmap::fromImage(img));
}

void AvManager::ffwdVideo()
{
	// seek forward of 5 secs.
	m_avThread->seekRequest(5.0);
}

void AvManager::rewVideo()
{
	// seek backward of 5 secs.
	m_avThread->seekRequest(-5.0);
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
