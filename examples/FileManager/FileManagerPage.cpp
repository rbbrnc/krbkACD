/*
 * FileManagerPage - Wrapper class to handle two FileManager classes.
 */
#include "FileManagerPage.h"
#include "ui_FileManagerPage.h"

#include <QDebug>
#include "FileManager.h"

FileManagerPage::FileManagerPage(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::FileManagerPage)
{
	ui->setupUi(this);

	m_fs1 = new FileManager();
	m_fs2 = new FileManager();

	ui->fs1Layout->addWidget(m_fs1);
	ui->fs2Layout->addWidget(m_fs2);

	ui->fs2Widget->hide();
	//m_fs1->showInfo(false);
	m_fs2->showInfo(false);

	// Set 1st as lastActive file manager
	m_lastActive = m_fs1;

	// XXX:
	// Use selection change of listViews to catch change of current
	// FileManager. (we can use focus??)
	connect(m_fs1, SIGNAL(currentChanged(const QString&)),
		this,  SLOT(setFileManager(const QString&)));

	connect(m_fs2, SIGNAL(currentChanged(const QString&)),
		this,  SLOT(setFileManager(const QString&)));
}

FileManagerPage::~FileManagerPage()
{
	delete m_fs1;
	delete m_fs2;
	delete ui;
}

void FileManagerPage::showSecondFs(bool checked)
{
	if (!checked) {
		m_lastActive = m_fs1;
	}

	ui->fs2Widget->setVisible(checked);
}

QString FileManagerPage::currentFile() const
{
	return m_lastActive->currentFile();
}

QString FileManagerPage::currentPath() const
{
	return m_lastActive->currentPath();
}

QString FileManagerPage::currentFilePath() const
{
	return m_lastActive->currentPath() + "/" + m_lastActive->currentFile();
}

// [SLOT private]
void FileManagerPage::showIcons(bool show)
{
	m_lastActive->iconMode(show);
}

// [SLOT private]
void FileManagerPage::showHidden(bool show)
{
	m_lastActive->showHidden(show);
}

// [SLOT private]
void FileManagerPage::showInfo(bool show)
{
	m_lastActive->showInfo(show);
}

// [public]
void FileManagerPage::updateInfo()
{
	m_fs1->updateInfo();
	m_fs2->updateInfo();
}

// [SLOT private]
void FileManagerPage::setFileManager(const QString &)
{
	FileManager *fm = static_cast<FileManager *>(sender());

	if (m_lastActive != fm) {
		m_lastActive = fm;
	}
}

// [SLOT private]
void FileManagerPage::previousFile()
{
	m_lastActive->previous();
}

// [SLOT private]
void FileManagerPage::nextFile()
{
	m_lastActive->next();
}

// [SLOT private]
void FileManagerPage::mkDir()
{
	m_lastActive->mkDir();
}

// [SLOT private]
void FileManagerPage::deleteFiles()
{
	m_lastActive->remove();
}

// [SLOT private]
void FileManagerPage::renameFiles()
{
	m_lastActive->rename();
}

// [SLOT private]
void FileManagerPage::copyFiles()
{
	// Copy only is 2nd browser is visible
	if (ui->fs2Widget->isHidden()) {
		return;
	}

	if (m_fs1 == m_lastActive) {
		m_fs1->copy(m_fs2->currentPath());
	} else if (m_fs2 == m_lastActive) {
		m_fs2->copy(m_fs1->currentPath());
	} else {
		return;
	}
}

// [SLOT private]
void FileManagerPage::moveFiles()
{
	// Move only is 2nd browser is visible
	if (ui->fs2Widget->isHidden()) {
		return;
	}

	if (m_fs1 == m_lastActive) {
		m_fs1->move(m_fs2->currentPath());
	} else if (m_fs2 == m_lastActive) {
		m_fs2->move(m_fs1->currentPath());
	} else {
		return;
	}
}

