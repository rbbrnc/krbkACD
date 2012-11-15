#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDebug>
#include "FileManager.h"
#include "FileManagerPage.h"
#include "ImageViewManager.h"
#include "MetadataTreeModel.h"
#include "QExiv2.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	m_fmPage = ui->fsManagerPage;
	m_ivPage = ui->ivManagerPage;
	m_mvPage = ui->metadataViewPage;

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
	connect(ui->actionViewMetadata,  SIGNAL(triggered()), this, SLOT(showMetadata()));

	connect(ui->actionFullScreen,  SIGNAL(triggered(bool)), this, SLOT(fullScreen(bool)));

	// So the shortcut works without the menu
	this->addAction(ui->actionFullScreen);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::fullScreen(bool enable)
{
	if (enable) {
		showFullScreen();
	} else {
		showNormal();
	}

	ui->menubar->setVisible(!enable);
//	ui->statusbar->setVisible(!enable);
//	ui->toolbar->setVisible(!enable);
}

void MainWindow::showImage()
{
	if (ui->stackedWidget->currentWidget() != m_ivPage) {
		m_ivPage->setFile(m_fmPage->currentFilePath());
//		qDebug() << __PRETTY_FUNCTION__ << m_fmPage->currentFilePath();
		ui->stackedWidget->setCurrentWidget(m_ivPage);
	} else {
		ui->stackedWidget->setCurrentWidget(m_fmPage);
	}
}

void MainWindow::showMetadata()
{
#if 0
	if (ui->stackedWidget->currentWidget() == m_fmPage) {
		// Setup model data.
		if (m_metadataModel) {
			delete m_metadataModel;
			m_metadataModel = 0;
		}

		if (m_currentFileData.isImage()) {
			m_metadataModel = new MetadataTreeModel(m_currentFileData.metadata());
			ui->metadataBrowser->setModel(m_metadataModel);
			ui->metadataBrowser->hideColumn(7); // hide key column
		}
		ui->stackedWidget->setCurrentWidget(m_mvPage);
	} else if (ui->stackedWidget->currentWidget() == m_mvPage) {
		ui->stackedWidget->setCurrentWidget(m_fmPage);
	}

	if (ui->stackedWidget->currentWidget() != m_mvPage) {
		m_ivPage->setFile(m_fmPage->currentFile(true));
		qDebug() << __PRETTY_FUNCTION__ << m_fmPage->currentFile(true);
		ui->stackedWidget->setCurrentWidget(m_mvPage);
	}
#endif
}

void MainWindow::prevFile()
{
	if (ui->stackedWidget->currentWidget() == m_ivPage) {
		m_fmPage->previousFile();
		m_ivPage->setFile(m_fmPage->currentFilePath());
	}
}

void MainWindow::nextFile()
{
	if (ui->stackedWidget->currentWidget() == m_ivPage) {
		m_fmPage->nextFile();
		m_ivPage->setFile(m_fmPage->currentFilePath());
	}
}

