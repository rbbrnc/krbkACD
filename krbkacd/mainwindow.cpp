#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "pages.h"
#include "MetadataDialog.h"
#include "ImageViewManager.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	m_prevPageIndex(0)
{
	ui->setupUi(this);

	m_browserPage = static_cast<BrowserPage *>(ui->stackedWidget->widget(BROWSER_PAGE));
	m_workPage    = static_cast<WorkPage *>(ui->stackedWidget->widget(WORK_PAGE));
	m_photoPage   = static_cast<ImageViewManager *>(ui->stackedWidget->widget(PHOTO_PAGE));
	m_duplicatePage = static_cast<DuplicatePage *>(ui->stackedWidget->widget(DUPLICATE_PAGE));

	// Connect changePage signals
	for (int i = 0; i < ui->stackedWidget->count(); i++) {
		connect(ui->stackedWidget->widget(i), SIGNAL(changePage(int)), this, SLOT(changePageSlot(int)));
	}

	// Connect Actions
	connect(ui->actionShowMetadata,      SIGNAL(toggled(bool)), m_browserPage, SLOT(showMetadata(bool)));
	connect(ui->actionShowPreview,       SIGNAL(toggled(bool)), m_browserPage, SLOT(showPreview(bool)));
	connect(ui->actionShowSecondBrowser, SIGNAL(toggled(bool)), m_browserPage, SLOT(showSecondBrowser(bool)));
	connect(ui->actionShowHidden,        SIGNAL(toggled(bool)), m_browserPage, SLOT(showHiddenFiles(bool)));
	connect(ui->actionShowIcons,         SIGNAL(toggled(bool)), m_browserPage, SLOT(showIcons(bool)));

	connect(ui->actionMkDir,             SIGNAL(triggered()),   m_browserPage, SLOT(makeNewDirectory()));
	connect(ui->actionCopyFile,          SIGNAL(triggered()),   m_browserPage, SLOT(copyFile()));
	connect(ui->actionMoveFile,          SIGNAL(triggered()),   m_browserPage, SLOT(moveFile()));

	connect(ui->actionDeleteFile, SIGNAL(triggered()), this, SLOT(deleteFile()));
	connect(ui->actionRenameFile, SIGNAL(triggered()), this, SLOT(renameFile()));

	//connect(ui->actionDebug, SIGNAL(triggered()), m_photoPage, SLOT(debugAction()));
	connect(ui->actionDebug, SIGNAL(triggered()), this, SLOT(debug()));

	connect(ui->actionQuit,           SIGNAL(triggered()), this, SLOT(close()));
	connect(ui->actionWork_Page,      SIGNAL(triggered()), this, SLOT(showWorkPage()));
	connect(ui->actionMetadataEditor, SIGNAL(triggered()), this, SLOT(showMetadataEditorPage()));
	connect(ui->actionFullScreen,     SIGNAL(triggered()), this, SLOT(fullScreen()));
/*
	connect(ui->actionZoom_1_1,             SIGNAL(triggered()), m_photoPage, SLOT(zoom11()));
	connect(ui->actionZoomToFit,            SIGNAL(triggered()), m_photoPage, SLOT(zoomToFit()));
	connect(ui->actionZoomIn,               SIGNAL(triggered()), m_photoPage, SLOT(zoomIn()));
	connect(ui->actionZoomOut,              SIGNAL(triggered()), m_photoPage, SLOT(zoomOut()));
	connect(ui->actionRotateCW,             SIGNAL(triggered()), m_photoPage, SLOT(rotateCW()));
	connect(ui->actionRotateCCW,            SIGNAL(triggered()), m_photoPage, SLOT(rotateCCW()));
	connect(ui->actionResetTransformations, SIGNAL(triggered()), m_photoPage, SLOT(resetTransformations()));
	connect(ui->actionShowRegions,          SIGNAL(toggled(bool)), m_photoPage, SLOT(showRegions(bool)));
*/
	connect(m_browserPage, SIGNAL(currentChanged()), this, SLOT(updateStatusBar()));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::updateStatusBar()
{
	QFileInfo fi = m_browserPage->currentFileData().fileInfo();
	QString mime = m_browserPage->currentFileData().mimeType();
	ui->statusBar->showMessage(QString("%1 %2 bytes").arg(mime).arg(fi.size()));
}

// SLOT for change UI pages
void MainWindow::changePageSlot(int page)
{
	if ((page < 0) || (page >= ui->stackedWidget->count())) {
		return;
	}

	switch (page) {
	case DUPLICATE_PAGE:
		m_duplicatePage->setMap(m_workPage->dupMap());
		break;
	default:
		break;
	}

	ui->stackedWidget->setCurrentIndex(page);
}

// Enable/Disable fullScreen Action
void MainWindow::enableFullScreenActions(bool enable)
{
	ui->actionZoom_1_1->setEnabled(enable);
	ui->actionZoomToFit->setEnabled(enable);
	ui->actionZoomIn->setEnabled(enable);
	ui->actionZoomOut->setEnabled(enable);

	ui->actionRotateCW->setEnabled(enable);
	ui->actionRotateCCW->setEnabled(enable);
	ui->actionResetTransformations->setEnabled(enable);

	ui->actionShowRegions->setEnabled(enable);
}

void MainWindow::fullScreen()
{
	switch (ui->stackedWidget->currentIndex()) {
	case BROWSER_PAGE:
	{
		QStringList files = m_browserPage->selectedFiles(QDir::Files);
		if (files.isEmpty()) {
			return;
		}

		// Speed up things if is a single file selection.
		if (files.count() == 1) {
			m_photoPage->setImage(m_browserPage->currentFileData().fullPixmap());
		} else {
			m_photoPage->setFiles(files);
		}

		m_prevPageIndex = BROWSER_PAGE;
		enableFullScreenActions(true);
		ui->stackedWidget->setCurrentWidget(m_photoPage);
		break;
	}
	case DUPLICATE_PAGE:
		m_prevPageIndex = DUPLICATE_PAGE;
		enableFullScreenActions(true);
		m_photoPage->setImage(m_duplicatePage->currentFileData().fullPixmap());
		ui->stackedWidget->setCurrentWidget(m_photoPage);
		break;
	case PHOTO_PAGE:
		// return to prev page
		enableFullScreenActions(false);
		ui->stackedWidget->setCurrentIndex(m_prevPageIndex);
		break;
	default:
		// Return to the browser page
		enableFullScreenActions(false);
		ui->stackedWidget->setCurrentWidget(m_browserPage);
		break;
	}
}

void MainWindow::showWorkPage()
{
	if (ui->stackedWidget->currentWidget() != m_workPage) {
		ui->stackedWidget->setCurrentWidget(m_workPage);
	} else {
		ui->stackedWidget->setCurrentWidget(m_browserPage);
	}
}

void MainWindow::showMetadataEditorPage()
{
#if 0
	switch (ui->stackedWidget->currentIndex()) {
	case BROWSER_PAGE:
		m_prevPageIndex = BROWSER_PAGE;
		m_metadataEditorPage->setFileData(m_browserPage->currentFileData());
		ui->stackedWidget->setCurrentWidget(m_metadataEditorPage);
		break;
	case DUPLICATE_PAGE:
		m_prevPageIndex = DUPLICATE_PAGE;
		m_metadataEditorPage->setFileData(m_duplicatePage->currentFileData());
		ui->stackedWidget->setCurrentWidget(m_metadataEditorPage);
		break;
	case PHOTO_PAGE:
		m_metadataEditorPage->setFileData(m_photoPage->fileData());
		ui->stackedWidget->setCurrentWidget(m_metadataEditorPage);
		break;
	case METADATA_EDITOR_PAGE:
		ui->stackedWidget->setCurrentIndex(m_prevPageIndex);
		break;
	default:
		// Return to the browser page
		ui->stackedWidget->setCurrentWidget(m_browserPage);
		break;
	}
#endif
}

void MainWindow::renameFile()
{
	switch (ui->stackedWidget->currentIndex()) {
	case BROWSER_PAGE:
		m_browserPage->renameFile();
		break;
	case DUPLICATE_PAGE:
		m_duplicatePage->renameFile();
		break;
	case PHOTO_PAGE:
		//m_photoPage->renameFile();
		break;
	default:
		break;
	}
}

void MainWindow::deleteFile()
{
	switch (ui->stackedWidget->currentIndex()) {
	case BROWSER_PAGE:
		m_browserPage->deleteFile();
		break;
	case DUPLICATE_PAGE:
		m_duplicatePage->deleteFile();
		break;
	case PHOTO_PAGE:
		//m_photoPage->deleteFile();
		break;
	default:
		break;
	}
}

void MainWindow::debug()
{
	FileData fd = m_browserPage->currentFileData();
	MetadataDialog dlg(fd.filePath(), fd.metadata());
	dlg.exec();
}
