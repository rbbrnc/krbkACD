#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "pages.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	m_prevPageIndex(0)
{
	ui->setupUi(this);

	m_browserPage = static_cast<BrowserPage *>(ui->stackedWidget->widget(0));
	m_workPage    = static_cast<WorkPage *>(ui->stackedWidget->widget(1));
	m_photoPage   = static_cast<PhotoWidget *>(ui->stackedWidget->widget(2));
	m_metadataEditorPage = static_cast<MetadataEditorPage *>(ui->stackedWidget->widget(3));
	m_duplicatePage = static_cast<DuplicatePage *>(ui->stackedWidget->widget(4));

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

	connect(ui->actionDebug, SIGNAL(triggered()), m_browserPage, SLOT(debugAction()));

	connect(ui->actionQuit,           SIGNAL(triggered()), this, SLOT(close()));
	connect(ui->actionWork_Page,      SIGNAL(triggered()), this, SLOT(showWorkPage()));
	connect(ui->actionMetadataEditor, SIGNAL(triggered()), this, SLOT(showMetadataEditorPage()));
	connect(ui->actionFullScreen,     SIGNAL(triggered()), this, SLOT(fullScreen()));

	connect(ui->actionZoom_1_1,             SIGNAL(triggered()), m_photoPage, SLOT(zoom11()));
	connect(ui->actionZoomToFit,            SIGNAL(triggered()), m_photoPage, SLOT(zoomToFit()));
	connect(ui->actionZoomIn,               SIGNAL(triggered()), m_photoPage, SLOT(zoomIn()));
	connect(ui->actionZoomOut,              SIGNAL(triggered()), m_photoPage, SLOT(zoomOut()));
	connect(ui->actionRotateCW,             SIGNAL(triggered()), m_photoPage, SLOT(rotateCW()));
	connect(ui->actionRotateCCW,            SIGNAL(triggered()), m_photoPage, SLOT(rotateCCW()));
	connect(ui->actionResetTransformations, SIGNAL(triggered()), m_photoPage, SLOT(resetTransformations()));
}

MainWindow::~MainWindow()
{
	delete ui;
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
}

void MainWindow::fullScreen()
{
	switch (ui->stackedWidget->currentIndex()) {
	case BROWSER_PAGE:
		m_prevPageIndex = BROWSER_PAGE;
		enableFullScreenActions(true);
		m_photoPage->setFileData(m_browserPage->currentFileData());
		ui->stackedWidget->setCurrentWidget(m_photoPage);
		break;
	case DUPLICATE_PAGE:
		m_prevPageIndex = DUPLICATE_PAGE;
		enableFullScreenActions(true);
		m_photoPage->setFileData(m_duplicatePage->currentFileData());
		ui->stackedWidget->setCurrentWidget(m_photoPage);
		break;
	case PHOTO_PAGE:
		// we need to return on prev page
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
	if (ui->stackedWidget->currentWidget() != m_metadataEditorPage) {
		m_metadataEditorPage->setFileData(m_browserPage->currentFileData());
		ui->stackedWidget->setCurrentWidget(m_metadataEditorPage);
	} else {
		ui->stackedWidget->setCurrentWidget(m_browserPage);
	}
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
		m_photoPage->renameFile();
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
		m_photoPage->deleteFile();
		break;
	default:
		break;
	}
}
