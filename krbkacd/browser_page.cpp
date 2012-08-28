#include "browser_page.h"
#include "ui_browser_page.h"

//#include <QDebug>

BrowserPage::BrowserPage(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::BrowserPage)
{
	ui->setupUi(this);

	// Start with single file browser
	ui->secondBrowser->setVisible(false);

	connect(ui->firstBrowser, SIGNAL(selectionChanged(QString)),
		this, SLOT(updateCurrentSelection(const QString)));
	connect(ui->secondBrowser, SIGNAL(selectionChanged(QString)),
		this, SLOT(updateCurrentSelection(const QString)));
	connect(ui->previewSplitter, SIGNAL(splitterMoved(int, int)),
		this, SLOT(previewSplitterMoved(int, int)));
}

BrowserPage::~BrowserPage()
{
	delete ui;
}

QPixmap BrowserPage::currentPixmap()
{
	return m_currentFileData.fullPixmap();
}

FileData BrowserPage::currentFileData() const
{
	return m_currentFileData;
}

QString BrowserPage::currentPath() const
{
	if (ui->secondBrowser->hasFocus()) {
		return ui->secondBrowser->getPath();
	} else {
		return ui->firstBrowser->getPath();
	}
}

void BrowserPage::keyPressEvent(QKeyEvent *e)
{
	if ((e->key() == Qt::Key_Control) || (e->key() == Qt::Key_Shift)) {
		if (ui->firstBrowser->hasFocus()) {
			ui->firstBrowser->setSelectionMode(QAbstractItemView::ExtendedSelection);
		} else if (ui->secondBrowser->hasFocus()) {
			ui->secondBrowser->setSelectionMode(QAbstractItemView::ExtendedSelection);
		}
	} else {
		QWidget::keyPressEvent(e);
	}
}

void BrowserPage::keyReleaseEvent(QKeyEvent *e)
{
	if ((e->key() == Qt::Key_Control) || (e->key() == Qt::Key_Shift)) {
		if (ui->firstBrowser->hasFocus()) {
			ui->firstBrowser->setSelectionMode(QAbstractItemView::SingleSelection);
		} else if (ui->secondBrowser->hasFocus()) {
			ui->secondBrowser->setSelectionMode(QAbstractItemView::SingleSelection);
		}
	} else {
		QWidget::keyPressEvent(e);
	}
}

void BrowserPage::showPreview(bool show)
{
	ui->previewPixmap->setVisible(show);
}

void BrowserPage::showHiddenFiles(bool show)
{
	ui->firstBrowser->actionShowHidden(show);
	ui->secondBrowser->actionShowHidden(show);
}

void BrowserPage::showIcons(bool show)
{
	ui->firstBrowser->actionSwitchMode(show);
	ui->secondBrowser->actionSwitchMode(show);
}

void BrowserPage::showMetadata(bool show)
{
	ui->metadataBrowser->setVisible(show);
}

void BrowserPage::showSecondBrowser(bool show)
{
	ui->secondBrowser->setVisible(show);
}

void BrowserPage::updatePreview()
{
	if (m_currentFileData.isImage()) {
		int h = ui->previewPixmap->size().height();
		int w = ui->previewPixmap->size().width();
		QPixmap pixScaled = m_currentFileData.previewPixmap(w, h);
		ui->previewPixmap->setPixmap(pixScaled);
	} else {
		ui->previewPixmap->setPixmap(NULL); //<- Null Pixmap
	}
}

void BrowserPage::updateCurrentSelection(const QString file)
{
	m_currentFileData = FileData(file);
	//m_currentFileData.print();

	ui->metadataBrowser->clearMetadata();

	updatePreview();

	if (m_currentFileData.isImage()) {
		ui->metadataBrowser->setFileData(m_currentFileData);
		ui->metadataBrowser->readMetadata();
	}
}

void BrowserPage::previewSplitterMoved(int, int)
{
	// update preview but not the metadata!
	updatePreview();
}

void BrowserPage::copyFile()
{
	// Copy only is 2nd browser is visible
	if (ui->secondBrowser->isHidden()) {
		return;
	}

	if (ui->firstBrowser->hasFocus()) {
		QString destPath = ui->secondBrowser->getPath();
		ui->firstBrowser->actionCopyFile(destPath);
	} else if (ui->secondBrowser->hasFocus()) {
		QString destPath = ui->firstBrowser->getPath();
		ui->secondBrowser->actionCopyFile(destPath);
	}
}

void BrowserPage::moveFile()
{
	// Move only is 2nd browser is visible
	if (ui->secondBrowser->isHidden()) {
		return;
	}

	if (ui->firstBrowser->hasFocus()) {
		QString destPath = ui->secondBrowser->getPath();
		ui->firstBrowser->actionMoveFile(destPath);
	} else if (ui->secondBrowser->hasFocus()) {
		QString destPath = ui->firstBrowser->getPath();
		ui->secondBrowser->actionMoveFile(destPath);
	}
}

void BrowserPage::deleteFile()
{
	if (ui->firstBrowser->hasFocus()) {
		ui->firstBrowser->actionDelete();
	} else if (ui->secondBrowser->hasFocus()) {
		ui->secondBrowser->actionDelete();
	}
}

void BrowserPage::renameFile()
{
	if (ui->firstBrowser->hasFocus()) {
		ui->firstBrowser->actionRename();
	} else if (ui->secondBrowser->hasFocus()) {
		ui->secondBrowser->actionRename();
	}
}

void BrowserPage::makeNewDirectory()
{
	if (ui->firstBrowser->hasFocus()) {
		QString destPath = ui->firstBrowser->getPath();
		ui->firstBrowser->actionMkDir(destPath);
	} else if (ui->secondBrowser->hasFocus()) {
		QString destPath = ui->secondBrowser->getPath();
		ui->secondBrowser->actionMkDir(destPath);
	}
}


