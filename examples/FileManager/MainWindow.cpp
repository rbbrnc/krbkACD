#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDebug>
#include "FileManager.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	m_fs1 = new FileManager();
	m_fs2 = new FileManager();

	ui->fs1Layout->addWidget(m_fs1);
	ui->fs2Layout->addWidget(m_fs2);
	m_fs2->hide();

	connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));

	connect(ui->actionShowSecondBrowser, SIGNAL(toggled(bool)), this, SLOT(showSecondFs(bool)));
	connect(ui->actionShowIcons, SIGNAL(toggled(bool)), m_fs1, SLOT(iconMode(bool)));
	connect(ui->actionShowIcons, SIGNAL(toggled(bool)), m_fs2, SLOT(iconMode(bool)));

	connect(ui->actionShowHidden, SIGNAL(toggled(bool)), m_fs1, SLOT(showHidden(bool)));
	connect(ui->actionShowHidden, SIGNAL(toggled(bool)), m_fs2, SLOT(showHidden(bool)));

	connect(ui->actionCopyFiles, SIGNAL(triggered()), this, SLOT(copyFiles()));
	connect(ui->actionMoveFiles, SIGNAL(triggered()), this, SLOT(moveFiles()));
	connect(ui->actionDeleteFiles, SIGNAL(triggered()), this, SLOT(deleteFiles()));
	connect(ui->actionRenameFiles, SIGNAL(triggered()), this, SLOT(renameFiles()));
	connect(ui->actionMkDir, SIGNAL(triggered()), this, SLOT(mkDir()));
	connect(ui->actionNextFile    , SIGNAL(triggered()), this, SLOT(nextFile()));
	connect(ui->actionPreviousFile, SIGNAL(triggered()), this, SLOT(previousFile()));
}

MainWindow::~MainWindow()
{
	delete m_fs1;
	delete m_fs2;
	delete ui;
}

void MainWindow::showSecondFs(bool checked)
{
	m_fs2->setVisible(checked);
}

FileManager *MainWindow::activeFileManager() const
{
	if (m_fs1->isActive()) {
		return m_fs1;
	} else if (m_fs2->isActive()) {
		return m_fs2;
	} else {
		return NULL;
	}
}

void MainWindow::previousFile()
{
	FileManager *fm = activeFileManager();
	if (fm) {
		fm->previous();
	}
}

void MainWindow::nextFile()
{
	FileManager *fm = activeFileManager();
	if (fm) {
		fm->next();
	}
}

void MainWindow::mkDir()
{
	FileManager *fm = activeFileManager();
	if (fm) {
		fm->mkDir();
	}
}

void MainWindow::deleteFiles()
{
	FileManager *fm = activeFileManager();
	if (fm) {
		fm->remove();
	}
}

void MainWindow::renameFiles()
{
	FileManager *fm = activeFileManager();
	if (fm) {
		fm->rename();
	}
}

void MainWindow::copyFiles()
{
	// Copy only is 2nd browser is visible
	if (m_fs2->isHidden()) {
		return;
	}

	FileManager *fm = activeFileManager();
	if (!fm) {
		return;
	}

	fm->copy((m_fs1->isActive()) ? m_fs2->currentPath() : m_fs1->currentPath());
}

void MainWindow::moveFiles()
{
	// Move only is 2nd browser is visible
	if (m_fs2->isHidden()) {
		return;
	}
	FileManager *fm = activeFileManager();
	if (!fm) {
		return;
	}

	fm->move((m_fs1->isActive()) ? m_fs2->currentPath() : m_fs1->currentPath());
}

