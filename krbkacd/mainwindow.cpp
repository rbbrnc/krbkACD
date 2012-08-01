#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "pages.h"

//#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent), ui(new Ui::MainWindow)
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

	connect(ui->actionCopyFile,          SIGNAL(triggered()),   m_browserPage, SLOT(copyFile()));
	connect(ui->actionMoveFile,          SIGNAL(triggered()),   m_browserPage, SLOT(moveFile()));
	connect(ui->actionDeleteFile,        SIGNAL(triggered()),   m_browserPage, SLOT(deleteFile()));
	connect(ui->actionRenameFile,        SIGNAL(triggered()),   m_browserPage, SLOT(renameFile()));
	connect(ui->actionMkDir,             SIGNAL(triggered()),   m_browserPage, SLOT(makeNewDirectory()));
	connect(ui->actionDebug,             SIGNAL(triggered()),   m_browserPage, SLOT(debugAction()));

	connect(ui->actionQuit,           SIGNAL(triggered()), this, SLOT(close()));
	connect(ui->actionWork_Page,      SIGNAL(triggered()), this, SLOT(showWorkPage()));
	connect(ui->actionMetadataEditor, SIGNAL(triggered()), this, SLOT(showMetadataEditorPage()));
	connect(ui->actionFullScreen,     SIGNAL(triggered()), this, SLOT(fullScreen()));

	connect(ui->actionZoom_1_1,             SIGNAL(triggered()), m_photoPage, SLOT(zoom11()));
	connect(ui->actionZoomToFit,            SIGNAL(triggered()), m_photoPage, SLOT(zoomToFit()));
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

void MainWindow::fullScreen()
{
	if (ui->stackedWidget->currentWidget() != m_photoPage) {
		// Enable fullScreen Action
		ui->actionZoom_1_1->setEnabled(true);
		ui->actionZoomToFit->setEnabled(true);
		ui->actionRotateCW->setEnabled(true);
		ui->actionRotateCCW->setEnabled(true);
		ui->actionResetTransformations->setEnabled(true);

		m_photoPage->setFileData(m_browserPage->currentFileData());
		ui->stackedWidget->setCurrentWidget(m_photoPage);
	} else {
		// Disable fullScreen Action
		ui->actionZoom_1_1->setEnabled(false);
		ui->actionZoomToFit->setEnabled(false);
		ui->actionRotateCW->setEnabled(false);
		ui->actionRotateCCW->setEnabled(false);
		ui->actionResetTransformations->setEnabled(false);

		// Return to the browser page
		ui->stackedWidget->setCurrentWidget(m_browserPage);
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
