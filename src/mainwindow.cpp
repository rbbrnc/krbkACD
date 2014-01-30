#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QDesktopServices>
#include <QUrl>

#include "FileManager.h"
#include "QMagic.h"
#include "LocationDialog.h"
#include "SocialMetadataDialog.h"
#include "metadatadialog.h"
#include "datetimedialog.h"

// Page Indexes
#define PAGE_INFO           0
#define PAGE_2ND_FM         1
#define PAGE_METADATA_TREE  2
#define PAGE_IMAGE_VIEW     3

#define EMPTY_SELECTION_TEXT "No File Selected!"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	m_fm  = new FileManager(ui->fmListView, this);
	ui->currentPath->setText(m_fm->currentPath());

	m_secondFm = new FileManager(ui->secondFmListView, this);

	// FileManager class signals
	connect(m_fm, SIGNAL(currentPathChanged()), this, SLOT(onCurrentPathChanged()));
	connect(m_fm, SIGNAL(currentChanged()),     this, SLOT(onCurrentFileChanged()));

	// App Actions
	connect(ui->actionQuit,       SIGNAL(triggered()),     this, SLOT(onQuit()));
	connect(ui->actionFullScreen, SIGNAL(triggered(bool)), this, SLOT(onFullScreen(bool)));

	ui->fmListView->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui->fmListView, SIGNAL(customContextMenuRequested(const QPoint &)),
		this, SLOT(showFmContextMenu(const QPoint &)));

	connect(ui->actionShowIcons,   SIGNAL(toggled(bool)), this, SLOT(onIconMode(bool)));
	connect(ui->actionShowHidden,  SIGNAL(toggled(bool)), this, SLOT(onShowHiddenFiles(bool)));
	connect(ui->actionDeleteFiles, SIGNAL(triggered()), this, SLOT(onDeleteFiles()));
	connect(ui->actionRenameFiles, SIGNAL(triggered()), this, SLOT(onRenameFiles()));
	connect(ui->actionMkDir,       SIGNAL(triggered()), this, SLOT(onMkDir()));
	connect(ui->actionCopyFiles,   SIGNAL(triggered()), this, SLOT(onCopyFiles()));
	connect(ui->actionMoveFiles,   SIGNAL(triggered()), this, SLOT(onMoveFiles()));
	connect(ui->actionOpenFile,    SIGNAL(triggered()), this, SLOT(onOpenFile()));

	// Metadata Edit Actions
	connect(ui->actionEditLocationsMetadata, SIGNAL(triggered()), this, SLOT(onEditLocationsMetadata()));
	connect(ui->actionEditSocialMetadata, SIGNAL(triggered()), this, SLOT(onEditSocialMetadata()));
	connect(ui->actionEditMetadata, SIGNAL(triggered()), this, SLOT(onEditMetadata()));
	connect(ui->actionEditDateTime, SIGNAL(triggered()), this, SLOT(onEditDateTimeMetadata()));

	m_pageGroup = new QActionGroup(this);
	m_pageGroup->addAction(ui->actionInfo);
	m_pageGroup->addAction(ui->action1);
	m_pageGroup->addAction(ui->action2);
	m_pageGroup->addAction(ui->action3);
	m_pageGroup->setExclusive(true);
	ui->actionInfo->setChecked(true);

	ui->actionInfo->setData(PAGE_INFO);
	ui->action1->setData(PAGE_2ND_FM);
	ui->action2->setData(PAGE_METADATA_TREE);
	ui->action3->setData(PAGE_IMAGE_VIEW);

	connect(ui->actionInfo, SIGNAL(toggled(bool)), this, SLOT(onChangePage(bool)));
	connect(ui->action1, SIGNAL(toggled(bool)), this, SLOT(onChangePage(bool)));
	connect(ui->action2, SIGNAL(toggled(bool)), this, SLOT(onChangePage(bool)));
	connect(ui->action3, SIGNAL(toggled(bool)), this, SLOT(onChangePage(bool)));

	// Connect splitters moved signal for resize preview
	connect(ui->previewSplitter, SIGNAL(splitterMoved(int, int)), this, SLOT(scalePreview(int, int)));

	// So the shortcut works without the menu
	this->addAction(ui->actionFullScreen);

//	m_secondFm->blockModelSignals(true);
}

MainWindow::~MainWindow()
{
	delete m_pageGroup;
	delete m_secondFm;
	delete m_fm;
	delete ui;
}

void MainWindow::onQuit()
{
	// TODO: save changes before close()
	close();
}

void MainWindow::onFullScreen(bool enable)
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

// SLOT
void MainWindow::on_previewCheckBox_stateChanged(int state)
{
	if (Qt::Checked == state) {
		updatePreview(m_fm->currentPath() + "/" + m_fm->currentFile());
		ui->preview->show();
	} else {
		ui->preview->hide();
	}
}

// SLOT (called only for main FileManager)
void MainWindow::onCurrentPathChanged()
{
	ui->currentPath->setText(m_fm->currentPath());
}

// SLOT (called only for main FileManager)
void MainWindow::onCurrentFileChanged()
{
	updatePageData(ui->stackedWidget->currentIndex());
}

void MainWindow::onChangePage(bool checked)
{
	QAction *action = static_cast<QAction *>(sender());
	int idx = action->data().toInt();

	if (checked) {
		// Show page 'idx'
		updatePageData(idx);
		ui->stackedWidget->setCurrentIndex(idx);
	} else {
		// Hide page 'idx'
		if (idx == PAGE_IMAGE_VIEW) {
			// Save region metadata if changed
			ui->imageViewPage->saveMetadata();
		}

		if (idx == PAGE_2ND_FM) {
			//m_secondFm->blockModelSignals(true);
		}
	}
}

void MainWindow::updatePageData(int page)
{
	QString file = m_fm->currentPath() + "/" + m_fm->currentFile();

	// Update Central Page Data
	switch (page) {
	case PAGE_INFO:
		ui->infoPage->setFile(file);
		break;
	case PAGE_2ND_FM:
		//m_secondFm->blockModelSignals(false);
		break;
	case PAGE_METADATA_TREE:
		ui->metadataTreePage->setFile(file);
		break;
	case PAGE_IMAGE_VIEW:
		ui->imageViewPage->saveMetadata();
		ui->imageViewPage->setImage(file, true);
		break;
	default:
		break;
	}

	if (ui->previewCheckBox->isChecked()) {
		updatePreview(file);
	}
}

// Slot called on previewSplitter moved signal
void MainWindow::scalePreview(int, int)
{
	// Don't scale null pixmaps
	if (!ui->previewCheckBox->isChecked() || m_preview.isNull()) {
		return;
	}

	if ((m_preview.width() > ui->preview->width()) || (m_preview.height() > ui->preview->height())) {
		QPixmap pix;
		pix = m_preview.scaled(ui->preview->width(), ui->preview->height(), Qt::KeepAspectRatio, Qt::FastTransformation);
		ui->preview->setPixmap(pix);
	}
}

void MainWindow::updatePreview(const QString &file)
{
	if ((file.isEmpty()) || (!QMagic::mimeTypeIsImage(file))) {
		m_preview = QPixmap();
		ui->preview->setPixmap(m_preview);
	} else {
		m_preview.load(file);
		if ((m_preview.width() > ui->preview->width()) ||
		    (m_preview.height() > ui->preview->height())) {
			// Scale pixmap to fit into preview widget
			QPixmap pix;
			pix = m_preview.scaled(ui->preview->width(), ui->preview->height(), Qt::KeepAspectRatio, Qt::FastTransformation);
			ui->preview->setPixmap(pix);
		} else {
			ui->preview->setPixmap(m_preview);
		}
	}
}

// [SLOT private]
void MainWindow::onCopyFiles()
{
	// Copy only is 2nd File manager is visible
	if (ui->stackedWidget->currentIndex() != PAGE_2ND_FM) {
		return;
	}
	if (m_fm->isActive()) {
		m_fm->copy(m_secondFm->currentPath());
	} else if (m_secondFm->isActive()) {
		m_secondFm->copy(m_fm->currentPath());
	} else {
		qDebug() << "CP Not Active";
	}
}

// [SLOT private]
void MainWindow::onMoveFiles()
{
	// Move only is 2nd file manager is visible
	if (ui->stackedWidget->currentIndex() != PAGE_2ND_FM) {
		return;
	}
	if (m_fm->isActive()) {
		m_fm->move(m_secondFm->currentPath());
	} else if (m_secondFm->isActive()) {
		m_secondFm->move(m_fm->currentPath());
	} else {
		qDebug() << "MV Not Active";
	}
}

void MainWindow::onIconMode(bool enable)
{
	m_fm->iconMode(enable);
	m_secondFm->iconMode(enable);
}

void MainWindow::onShowHiddenFiles(bool enable)
{
	m_fm->showHidden(enable);
	m_secondFm->showHidden(enable);
}

void MainWindow::onDeleteFiles()
{
	if (m_fm->isActive()) {
		m_fm->deleteSelectedFiles();
	} else if (m_secondFm->isActive()) {
		m_secondFm->deleteSelectedFiles();
	}
}

void MainWindow::onRenameFiles()
{
	if (m_fm->isActive()) {
		m_fm->renameSelectedFiles();
	} else if (m_secondFm->isActive()) {
		m_secondFm->renameSelectedFiles();
	}
}

void MainWindow::onMkDir()
{
	if (m_fm->isActive()) {
		m_fm->mkdir();
	} else if (m_secondFm->isActive()) {
		m_secondFm->mkdir();
	}
}

void MainWindow::onEditLocationsMetadata()
{
	QStringList files = fileSelection();
	if (files.count() == 0) {
		QMessageBox::warning(this, "Edit Locations", "No File Selected!");
	} else {
		LocationDialog dlg(files);
		dlg.exec();
	}
}

void MainWindow::onEditSocialMetadata()
{
	QStringList files = fileSelection();
	if (files.count() == 0) {
		QMessageBox::warning(this, "Edit Social Metadata", "No File Selected!");
	} else {
		SocialMetadataDialog dlg(files);
		dlg.exec();
	}
}

void MainWindow::onEditMetadata()
{
	QStringList files = fileSelection();
	if (files.count() == 0) {
		QMessageBox::warning(this, "Edit Metadata", "No File Selected!");
	} else {
		MetadataDialog dlg(files);
		dlg.exec();
	}
}

void MainWindow::onEditDateTimeMetadata()
{
	QStringList files = fileSelection();
	if (files.count() == 0) {
		QMessageBox::warning(this, "Edit DateTime Metadata", "No File Selected!");
	} else {
		MetadataDateTimeDialog dlg(files);
		dlg.exec();
	}
}

QStringList MainWindow::fileSelection() const
{
	if (m_fm->isActive()) {
		return m_fm->fileSelection();
	} else if (m_secondFm->isActive()) {
		return m_secondFm->fileSelection();
	} else {
		return QStringList();
	}
}

// Context menu for file-object entries
void MainWindow::showFmContextMenu(const QPoint &pos)
{
	QMenu contextMenu(tr("Context menu"), this);
    contextMenu.addAction(ui->actionEditSocialMetadata);
	contextMenu.addAction(ui->actionOpenFile);
	contextMenu.addAction(ui->actionRenameFiles);
	contextMenu.addAction(ui->actionCopyFiles);
	contextMenu.addAction(ui->actionMoveFiles);
	contextMenu.addAction(ui->actionDeleteFiles);
	contextMenu.exec(mapToGlobal(pos));
}

void MainWindow::onOpenFile()
{
	QStringList files = fileSelection();

	// USE DESKTOP SERVICES TO OPEN FILES (xdg-open)!
	// The temporary files are deleted when the application is closed!
	if (!QDesktopServices::openUrl(QUrl::fromLocalFile(files.at(0)))) {
		qDebug() << __PRETTY_FUNCTION__ << "Cannot open Url:";
	}
}

