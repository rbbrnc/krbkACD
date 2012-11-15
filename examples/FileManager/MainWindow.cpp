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

	m_exiv2 = new QExiv2();
}

MainWindow::~MainWindow()
{
	delete m_exiv2;
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
//
		QList<PTag> tagList;
		if (m_exiv2->load(m_fmPage->currentFilePath())) {
			tagList = m_exiv2->xmpPTags();
			if (tagList.isEmpty()) {
				// Check MWG regions
				tagList = m_exiv2->xmpMWG_RegionsTags();
			}

			if (!tagList.isEmpty()) {
				QList<QRectF> regions;
				for (int i = 0; i < tagList.size(); i++) {
					regions << tagList.at(i).region();
				}
				m_ivPage->addRectRegions(regions);
			}
		}

/*
		regions << QRectF(10.0, 10.0, 25, 25);
		regions << QRectF(50.0, 50.0, 25, 25);
*/

//		m_ivPage->addRectRegion(QRectF(10.0, 10.0, 25, 25));

		ui->stackedWidget->setCurrentWidget(m_ivPage);
	} else {
		qDebug() << m_ivPage->rectRegions();
		ui->stackedWidget->setCurrentWidget(m_fmPage);
	}
}

void MainWindow::showMetadata()
{
	if (ui->stackedWidget->currentWidget() != m_mvPage) {
//		m_mvPage->setFile(m_fmPage->currentFilePath());
//		qDebug() << __PRETTY_FUNCTION__ << m_fmPage->currentFilePath();
		ui->stackedWidget->setCurrentWidget(m_mvPage);
	} else {
		ui->stackedWidget->setCurrentWidget(m_fmPage);
	}
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

