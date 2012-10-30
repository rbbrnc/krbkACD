#include "FileManagerPage.h"
#include "ui_FileManagerPage.h"

#include <QDebug>
#include "FileManager.h"

FileManagerPage::FileManagerPage(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::FileManagerPage)
{
	ui->setupUi(this);

	m_lastActive = NULL;
	m_fs1 = new FileManager();
	m_fs2 = new FileManager();

	ui->fs1Layout->addWidget(m_fs1);
	ui->fs2Layout->addWidget(m_fs2);
	m_fs2->hide();

	m_fs1->showInfo(false);
	m_fs2->showInfo(false);
}

FileManagerPage::~FileManagerPage()
{
	delete m_fs1;
	delete m_fs2;
	delete ui;
}

void FileManagerPage::showSecondFs(bool checked)
{
	m_fs2->setVisible(checked);
}

QString FileManagerPage::currentFile(bool useLastActiveFm) const
{
	FileManager *fm = activeFileManager(useLastActiveFm);
	if (fm) {
		return fm->currentFile();
	}

	qDebug() << __PRETTY_FUNCTION__ << "Invalid FileManager";
	return QString();
}

QString FileManagerPage::currentPath(bool useLastActiveFm) const
{
	FileManager *fm = activeFileManager(useLastActiveFm);
	if (fm) {
		return fm->currentPath();
	}
	return QString();
}

QString FileManagerPage::currentFilePath(bool useLastActiveFm) const
{
	FileManager *fm = activeFileManager(useLastActiveFm);
	if (fm) {
		return fm->currentPath() + "/" + fm->currentFile();
	}

	qDebug() << __PRETTY_FUNCTION__ << "Invalid FileManager";
	return QString();
}

void FileManagerPage::showIcons(bool show)
{
	FileManager *fm = activeFileManager();
	if (fm) {
		fm->iconMode(show);
	}
}

void FileManagerPage::showHidden(bool show)
{
	FileManager *fm = activeFileManager();
	if (fm) {
		fm->showHidden(show);
	}
}

void FileManagerPage::showInfo(bool show)
{
	FileManager *fm = activeFileManager();
	if (fm) {
		fm->showInfo(show);
	}
}

void FileManagerPage::setActiveFileManager()
{
	m_lastActive = activeFileManager(false);
}

FileManager *FileManagerPage::activeFileManager(bool useLastActive) const
{
	if (m_fs1->isActive()) {
		return m_fs1;
	} else if (m_fs2->isActive()) {
		return m_fs2;
	} else {
		if (useLastActive) {
			return (m_lastActive) ? m_lastActive : NULL;
		}
	}
	return NULL;
}

void FileManagerPage::previousFile(bool useLastActiveFm)
{
	FileManager *fm = activeFileManager(useLastActiveFm);
	if (fm) {
		fm->previous();
	}
}

void FileManagerPage::nextFile(bool useLastActiveFm)
{
	FileManager *fm = activeFileManager(useLastActiveFm);
	if (fm) {
		fm->next();
	}
}

void FileManagerPage::mkDir()
{
	FileManager *fm = activeFileManager();
	if (fm) {
		fm->mkDir();
	}
}

void FileManagerPage::deleteFiles()
{
	FileManager *fm = activeFileManager();
	if (fm) {
		fm->remove();
	}
}

void FileManagerPage::renameFiles()
{
	FileManager *fm = activeFileManager();
	if (fm) {
		fm->rename();
	}
}

void FileManagerPage::copyFiles()
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

void FileManagerPage::moveFiles()
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

