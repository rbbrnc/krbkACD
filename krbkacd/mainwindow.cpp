#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	browserPage    = static_cast<BrowserPage *>(ui->stackedWidget->widget(0));
	fullScreenPage = static_cast<FullScreenPage *>(ui->stackedWidget->widget(1));
	workPage       = static_cast<WorkPage *>(ui->stackedWidget->widget(2));
	m_photoPage = static_cast<PhotoWidget *>(ui->stackedWidget->widget(3));

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

	// For debugging Browser Page
	connect(ui->actionDebug, SIGNAL(triggered()), browserPage, SLOT(debugAction()));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::fullScreen()
{
#if 10
	if (ui->stackedWidget->currentWidget() != m_photoPage) {
		m_photoPage->setPixmap(browserPage->currentPixmap());
		ui->stackedWidget->setCurrentWidget(m_photoPage);
	} else {
		ui->stackedWidget->setCurrentWidget(browserPage);
	}
#else
	if (ui->stackedWidget->currentWidget() != fullScreenPage) {
		fullScreenPage->setPixmap(browserPage->currentPixmap());
		ui->stackedWidget->setCurrentWidget(fullScreenPage);
	} else {
		ui->stackedWidget->setCurrentWidget(browserPage);
	}
#endif
}

void MainWindow::showWorkPage()
{
	if (ui->stackedWidget->currentWidget() != workPage) {
		ui->stackedWidget->setCurrentWidget(workPage);
	} else {
		ui->stackedWidget->setCurrentWidget(browserPage);
	}
}
