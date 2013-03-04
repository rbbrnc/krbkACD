#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(const QString &file, QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	m_file(file)
{
	ui->setupUi(this);

	m_process = new QProcess(this);
	connect(ui->startButton, SIGNAL(clicked()), this, SLOT(startProcess()));
	connect(ui->pauseButton, SIGNAL(clicked()), this, SLOT(pausePlayer()));
	connect(ui->stopButton, SIGNAL(clicked()), this, SLOT(stopPlayer()));


	connect(m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(readStdout()));
	connect(m_process, SIGNAL(readyReadStandardError()),  this, SLOT(readStderr()));
}

MainWindow::~MainWindow()
{
	delete ui;

	quitPlayer();
	delete m_process;
}

void MainWindow::readStdout()
{
	QStringList lines = QString::fromLocal8Bit(m_process->readAllStandardOutput()).split("\n", QString::SkipEmptyParts);
	for (int i = 0; i < lines.count(); i++) {
		lines[i].remove("\r");
		//qDebug("out: \"%s\"", qPrintable(lines[i]));
		parsePlayerOutputLine(lines[i]);
		//emit readStandardOutput(lines[i]);
	}
}

void MainWindow::readStderr()
{
	QStringList lines = QString::fromLocal8Bit(m_process->readAllStandardError()).split("\n", QString::SkipEmptyParts);
	for (int i = 0; i < lines.count(); i++) {
		lines[i].remove("\r");
		//qDebug("err: \"%s\"", qPrintable(lines[i]));
		parsePlayerOutputLine(lines[i]);
		//emit readStandardError(lines[i]);
	}
}

void MainWindow::parsePlayerOutputLine(const QString &line)
{
	static QRegExp rx_av("^[AV]: *([0-9,:.-]+)");
	static QRegExp rx_frame("^[AV]:.* (\\d+)\\/.\\d+");// [0-9,.]+");

	if (rx_av.indexIn(line) > -1) {
		double sec = rx_av.cap(1).toDouble();
		//qDebug("cap(1): '%s'", rx_av.cap(1).toUtf8().data() );
		qDebug("sec: %f", sec);
	}

	// Check for frame
	if (rx_frame.indexIn(line) > -1) {
		int frame = rx_frame.cap(1).toInt();
		qDebug(" frame: %d", frame);
		//emit receivedCurrentFrame(frame);
	}

#if 0
	if (line.startsWith("A:") || line.startsWith("V:")) {
		static QRegExp rx("[ :]");
		QStringList info = line.split(rx, QString::SkipEmptyParts);
		double oldpos = m_streamPosition;
		for (int i = 0; i < info.count(); i++) {
			if ((info[i] == "V" || info[i] == "A") && info.count() > i) {
				m_streamPosition = info[i + 1].toDouble();

				// If the movie is near its end, start a timer that will check whether
				// the movie has really finished.
				//if (qAbs(m_streamPosition - m_mediaInfo.length) < 1) {
				//	m_movieFinishedTimer.start();
				//}
			}
		}

		if (oldpos != m_streamPosition) {
			//emit streamPositionChanged(m_streamPosition);
		}
	}
        //parsePosition(line);
        qDebug() << m_streamPosition;
#endif
}

void MainWindow::pausePlayer()
{
	m_process->write("pause\n");
	m_process->write("stream_time_pos\n");
//frame_step

}

void MainWindow::stopPlayer()
{
	m_process->write("stop\n");
}

void MainWindow::quitPlayer()
{
	m_process->write("quit\n");
	m_process->waitForFinished(100);
	if (m_process->state() == QProcess::Running) {
		m_process->kill();
	}
	m_process->waitForFinished(-1);
}

void MainWindow::startProcess()
{
	qDebug() << winId() << m_file;

	if (m_process->state() == QProcess::Running) {
		quitPlayer();
	}

	m_streamPosition = 0;

	//QString winid = "-wid " + QString::number((int) winId());

	QStringList arguments;
	//arguments << "mplayer" << winid << m_file;
	//arguments << "mplayer" << "-wid" << QString::number((int) winId()) << m_file;
	arguments << "-nogui" << "-slave" << "-idle" << "-wid" << QString::number((int) ui->processWidget->winId()) << m_file;
	qDebug() << arguments;

//	m_process->start("mrxvt -into "+QString::number((int)ui->processWidget->winId()));
//	m_process->start("mplayer -wid " + QString::number((int)ui->processWidget->winId()));

	m_process->start("mplayer", arguments);

}
