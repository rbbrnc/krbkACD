#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	browserPage = static_cast<BrowserPage *>(ui->stackedWidget->widget(0));
	workPage    = static_cast<WorkPage *>(ui->stackedWidget->widget(1));
	m_photoPage = static_cast<PhotoWidget *>(ui->stackedWidget->widget(2));

	// Connect View Actions
	connect(ui->actionShowMetadata, SIGNAL(toggled(bool)),
		browserPage, SLOT(showMetadata(bool)));
	connect(ui->actionShowPreview, SIGNAL(toggled(bool)),
		browserPage, SLOT(showPreview(bool)));
	connect(ui->actionShowSecondBrowser, SIGNAL(toggled(bool)),
		browserPage, SLOT(showSecondBrowser(bool)));
	connect(ui->actionShowHidden, SIGNAL(toggled(bool)),
		browserPage, SLOT(showHiddenFiles(bool)));
	connect(ui->actionShowIcons, SIGNAL(toggled(bool)),
		browserPage, SLOT(showIcons(bool)));

	connect(ui->actionWork_Page, SIGNAL(triggered()), this, SLOT(showWorkPage()));

	// Connect File Action
	connect(ui->actionQuit,       SIGNAL(triggered()), this, SLOT(close()));
	connect(ui->actionCopyFile,   SIGNAL(triggered()), browserPage, SLOT(copyFile()));
	connect(ui->actionMoveFile,   SIGNAL(triggered()), browserPage, SLOT(moveFile()));
	connect(ui->actionDeleteFile, SIGNAL(triggered()), browserPage, SLOT(deleteFile()));
	connect(ui->actionRenameFile, SIGNAL(triggered()), browserPage, SLOT(renameFile()));

	// Connect Image Actions
	connect(ui->actionFullScreen, SIGNAL(triggered()), this, SLOT(fullScreen()));

	connect(ui->actionZoom_1_1,  SIGNAL(triggered()), m_photoPage, SLOT(zoom11()));
	connect(ui->actionZoomToFit, SIGNAL(triggered()), m_photoPage, SLOT(zoomToFit()));

	connect(ui->actionRotateCW, SIGNAL(triggered()), m_photoPage, SLOT(rotateCW()));
	connect(ui->actionRotateCCW, SIGNAL(triggered()), m_photoPage, SLOT(rotateCCW()));
	connect(ui->actionResetTransformations, SIGNAL(triggered()),
		m_photoPage, SLOT(resetTransformations()));

	// For debugging Browser Page
	connect(ui->actionDebug, SIGNAL(triggered()), browserPage, SLOT(debugAction()));
}

MainWindow::~MainWindow()
{
	delete ui;
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

		m_photoPage->setFileData(browserPage->currentFileData());
		ui->stackedWidget->setCurrentWidget(m_photoPage);
	} else {
		// Disable fullScreen Action
		ui->actionZoom_1_1->setEnabled(false);
		ui->actionZoomToFit->setEnabled(false);
		ui->actionRotateCW->setEnabled(false);
		ui->actionRotateCCW->setEnabled(false);
		ui->actionResetTransformations->setEnabled(false);

		// Return to the browser page
		ui->stackedWidget->setCurrentWidget(browserPage);
	}
}

void MainWindow::showWorkPage()
{
	if (ui->stackedWidget->currentWidget() != workPage) {
		ui->stackedWidget->setCurrentWidget(workPage);
	} else {
		ui->stackedWidget->setCurrentWidget(browserPage);
	}
}
