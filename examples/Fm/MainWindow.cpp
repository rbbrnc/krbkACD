#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>
#include "FileManager.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	m_fm = new FileManager(ui->listView, this);

/*
	m_fmPage = ui->fsManagerPage;
	m_mvPage = ui->metadataViewPage;
	ui->stackedWidget->setCurrentWidget(m_fmPage);
*/
	connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));

/*
	connect(ui->actionShowSecondBrowser, SIGNAL(toggled(bool)), m_fmPage, SLOT(showSecondFs(bool)));

	connect(ui->actionShowInfo,   SIGNAL(toggled(bool)), m_fmPage, SLOT(showInfo(bool)));
*/
	// Connect actions handled by FileManager class.
	connect(ui->actionShowIcons,   SIGNAL(toggled(bool)), m_fm, SLOT(iconMode(bool)));
	connect(ui->actionShowHidden,  SIGNAL(toggled(bool)), m_fm, SLOT(showHidden(bool)));
	connect(ui->actionMkDir,       SIGNAL(triggered()), m_fm, SLOT(mkdir()));
	connect(ui->actionDeleteFiles, SIGNAL(triggered()), m_fm, SLOT(deleteSelectedFiles()));
	connect(ui->actionRenameFiles, SIGNAL(triggered()), m_fm, SLOT(renameSelectedFiles()));

//	connect(ui->actionCopyFiles, SIGNAL(triggered()), m_fmPage, SLOT(copyFiles()));
//	connect(ui->actionMoveFiles, SIGNAL(triggered()), m_fmPage, SLOT(moveFiles()));

/*
	// Connect next/prev actions.
	connect(ui->actionPreviousFile, SIGNAL(triggered()), this, SLOT(prevFile()));
	connect(ui->actionNextFile    , SIGNAL(triggered()), this, SLOT(nextFile()));

	// Connect next/prev requests coming from ImageViewManager.
	connect(m_ivPage, SIGNAL(requestPreviousFile()), this, SLOT(prevFile()));
	connect(m_ivPage, SIGNAL(requestNextFile()),     this, SLOT(nextFile()));

	// Connect next/prev requests coming from MetadataTreeViewPage.
	connect(m_mvPage, SIGNAL(requestPreviousFile()), this, SLOT(prevFile()));
	connect(m_mvPage, SIGNAL(requestNextFile()),     this, SLOT(nextFile()));

	connect(ui->actionViewImage,    SIGNAL(triggered()), this, SLOT(showImage()));
	connect(ui->actionViewMetadata, SIGNAL(triggered()), this, SLOT(showMetadata()));
	connect(ui->actionViewImageRegions, SIGNAL(triggered(bool)), this, SLOT(showImageRegions(bool)));

	connect(ui->actionFullScreen, SIGNAL(triggered(bool)), this, SLOT(fullScreen(bool)));

	// So the shortcut works without the menu
	this->addAction(ui->actionFullScreen);
*/
}

MainWindow::~MainWindow()
{
	delete ui;
	delete m_fm;
}

void MainWindow::showImageRegions(bool enable)
{
//	m_ivPage->showRegions(enable);
}

void MainWindow::fullScreen(bool enable)
{
/*
	if (enable) {
		showFullScreen();
	} else {
		showNormal();
	}

	ui->menubar->setVisible(!enable);
//	ui->statusbar->setVisible(!enable);
//	ui->toolbar->setVisible(!enable);
*/
}

void MainWindow::showImage()
{
/*
	if (ui->stackedWidget->currentWidget() != m_ivPage) {
		loadImage(m_fmPage->currentFilePath(), true);
		ui->stackedWidget->setCurrentWidget(m_ivPage);
	} else {
		ui->stackedWidget->setCurrentWidget(m_fmPage);
		// Update info panel
		m_fmPage->updateInfo();
	}
*/
}

void MainWindow::showMetadata()
{
/*
	if (ui->stackedWidget->currentWidget() != m_mvPage) {
		m_mvPage->setFile(m_fmPage->currentFilePath());
		ui->stackedWidget->setCurrentWidget(m_mvPage);
	} else {
		ui->stackedWidget->setCurrentWidget(m_fmPage);
	}
*/
}

void MainWindow::prevFile()
{
/*
	if (ui->stackedWidget->currentWidget() == m_ivPage) {
		m_fmPage->previousFile();
		loadImage(m_fmPage->currentFilePath(), true);
	}

	if (ui->stackedWidget->currentWidget() == m_mvPage) {
		m_fmPage->previousFile();
		m_mvPage->setFile(m_fmPage->currentFilePath());
	}
*/
}

void MainWindow::nextFile()
{
/*
	if (ui->stackedWidget->currentWidget() == m_ivPage) {
		m_fmPage->nextFile();
		loadImage(m_fmPage->currentFilePath(), true);
	}

	if (ui->stackedWidget->currentWidget() == m_mvPage) {
		m_fmPage->nextFile();
		m_mvPage->setFile(m_fmPage->currentFilePath());
	}
*/
}

void MainWindow::loadImage(const QString &fileName, bool loadMetadata)
{
#if 0
	m_ivPage->setImage(fileName);
        if (loadMetadata) {
                QExiv2 *e = new QExiv2();
                if (e->load(fileName)) {
                        if (e->xmpHasRegionTags()) {
                                // Get XMP Image Regions
                                MwgRegionList rl = e->xmpMwgRegionList();
                                for (int i = 0; i < rl.count(); i++) {
					m_ivPage->insertRegion(rl.at(i).stAreaBoundingRectF(),
							       rl.at(i).name(),
							       rl.at(i).description());

                                        qDebug() << __PRETTY_FUNCTION__
                                                 << rl.at(i).stAreaBoundingRectF()
                                                 << rl.at(i).stArea()
                                                 << rl.at(i).name()
                                                 << rl.at(i).description();
                                }
                        }
                }
		delete e;
        }
#endif
}
