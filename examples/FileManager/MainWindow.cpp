#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDebug>
#include "FileManager.h"
#include "FileManagerPage.h"
#include "ImageViewManager.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	m_fmPage = ui->fsManagerPage;
	m_ivPage = ui->ivManagerPage;

	ui->stackedWidget->setCurrentWidget(m_fmPage);

	connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));

	connect(ui->actionShowSecondBrowser, SIGNAL(toggled(bool)), m_fmPage, SLOT(showSecondFs(bool)));

	connect(ui->actionShowIcons,  SIGNAL(toggled(bool)), m_fmPage, SLOT(showIcons(bool)));
	connect(ui->actionShowHidden, SIGNAL(toggled(bool)), m_fmPage, SLOT(showHidden(bool)));
	connect(ui->actionShowInfo,   SIGNAL(toggled(bool)), m_fmPage, SLOT(showInfo(bool)));

	connect(ui->actionCopyFiles, SIGNAL(triggered()), m_fmPage, SLOT(copyFiles()));
	connect(ui->actionMoveFiles, SIGNAL(triggered()), m_fmPage, SLOT(moveFiles()));
	connect(ui->actionDeleteFiles, SIGNAL(triggered()), m_fmPage, SLOT(deleteFiles()));
	connect(ui->actionRenameFiles, SIGNAL(triggered()), m_fmPage, SLOT(renameFiles()));
	connect(ui->actionMkDir, SIGNAL(triggered()), m_fmPage, SLOT(mkDir()));

	connect(ui->actionPreviousFile, SIGNAL(triggered()), this, SLOT(prevFile()));
	connect(ui->actionNextFile    , SIGNAL(triggered()), this, SLOT(nextFile()));

	connect(ui->actionViewImage,  SIGNAL(triggered()), this, SLOT(showImage()));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::showImage()
{
	if (ui->stackedWidget->currentWidget() != m_ivPage) {
		m_fmPage->setActiveFileManager();
		m_ivPage->setFile(m_fmPage->currentFile(true));
		ui->stackedWidget->setCurrentWidget(m_ivPage);
	} else {
		ui->stackedWidget->setCurrentWidget(m_fmPage);
	}
}

void MainWindow::prevFile()
{
	if (ui->stackedWidget->currentWidget() == m_ivPage) {
		m_fmPage->previousFile(true);
		m_ivPage->setFile(m_fmPage->currentFile(true));
	}
}

void MainWindow::nextFile()
{
	if (ui->stackedWidget->currentWidget() == m_ivPage) {
		m_fmPage->nextFile(true);
		m_ivPage->setFile(m_fmPage->currentFile(true));
	}
}

