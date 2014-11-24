#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QDesktopServices>
#include <QUrl>

#include "version.h"
#include "FileManager.h"
#include "QMagic.h"
#include "md_socialdialog.h"
#include "md_keywordsdialog.h"
#include "md_datetimedialog.h"
#include "md_locationdialog.h"

#include "metadatadialog.h"

// Page Indexes
#define PAGE_INFO            0
#define PAGE_2ND_FM          1
#define PAGE_METADATA_TREE   2
#define PAGE_IMAGE_VIEW      3
#define PAGE_METADATA_EDITOR 4

//#define EMPTY_SELECTION_TEXT "No File Selected!"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	ui->fmListView->setLayoutMode(QListView::Batched);

    m_fm1 = new FileManager(ui->fmListView, this);
    m_fm2 = new FileManager(ui->secondFmListView, this);

    ui->currentPath->setText(m_fm1->currentPath());


	// FileManager class signals
    // using [=] to capture variables used in the body by value
    connect(m_fm1, &FileManager::currentPathChanged, [=] { ui->currentPath->setText(m_fm1->currentPath()); });
    connect(m_fm1, &FileManager::currentChanged, [=] { updatePageData(ui->stackedWidget->currentIndex()); });

	// App Actions
    connect(ui->actionQuit,       &QAction::triggered, this, &MainWindow::onQuit);
    connect(ui->actionFullScreen, &QAction::triggered, this, &MainWindow::onFullScreen);

	ui->fmListView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->fmListView, &QListView::customContextMenuRequested, this, &MainWindow::showFmContextMenu);

    // Show Icons/List
    connect(ui->actionShowIcons, &QAction::toggled, m_fm1, &FileManager::iconMode);
    connect(ui->actionShowIcons, &QAction::toggled, m_fm2, &FileManager::iconMode);

    // Show/Hide Hidden Files
    connect(ui->actionShowHidden, &QAction::toggled, m_fm1, &FileManager::showHidden);
    connect(ui->actionShowHidden, &QAction::toggled, m_fm2, &FileManager::showHidden);

    // Delete Files
    connect(ui->actionDeleteFiles, &QAction::triggered, m_fm1, &FileManager::deleteSelectedFiles);
    connect(ui->actionDeleteFiles, &QAction::triggered, m_fm2, &FileManager::deleteSelectedFiles);

    // Rename Files
    connect(ui->actionRenameFiles, &QAction::triggered, m_fm1, &FileManager::renameSelectedFiles);
    connect(ui->actionRenameFiles, &QAction::triggered, m_fm2, &FileManager::renameSelectedFiles);

    // MkDir
    connect(ui->actionMkDir, &QAction::triggered, m_fm1, &FileManager::mkdir);
    connect(ui->actionMkDir, &QAction::triggered, m_fm2, &FileManager::mkdir);

    connect(ui->actionCopyFiles, &QAction::triggered, this, &MainWindow::onCopyFiles);
    connect(ui->actionMoveFiles, &QAction::triggered, this, &MainWindow::onMoveFiles);
    connect(ui->actionOpenFile,  &QAction::triggered, this, &MainWindow::onOpenFile);

	// Metadata Edit Actions
    connect(ui->actionEditLocationsMetadata, &QAction::triggered, this, &MainWindow::onEditLocationsMetadata);
    connect(ui->actionEditSocialMetadata, &QAction::triggered, this, &MainWindow::onEditSocialMetadata);
    connect(ui->actionEditMetadata, &QAction::triggered, this, &MainWindow::onEditMetadata);
    connect(ui->actionEditDateTime, &QAction::triggered, this, &MainWindow::onEditDateTimeMetadata);

    m_pageGroup = new QActionGroup(this);
	m_pageGroup->addAction(ui->actionInfo);
	m_pageGroup->addAction(ui->action1);
	m_pageGroup->addAction(ui->action2);
	m_pageGroup->addAction(ui->action3);
    m_pageGroup->addAction(ui->action4);
	m_pageGroup->setExclusive(true);
	ui->actionInfo->setChecked(true);

	ui->actionInfo->setData(PAGE_INFO);
	ui->action1->setData(PAGE_2ND_FM);
	ui->action2->setData(PAGE_METADATA_TREE);
	ui->action3->setData(PAGE_IMAGE_VIEW);
    ui->action4->setData(PAGE_METADATA_EDITOR);

    connect(ui->actionInfo, &QAction::toggled, this, &MainWindow::onChangePage);
    connect(ui->action1, &QAction::toggled, this, &MainWindow::onChangePage);
    connect(ui->action2, &QAction::toggled, this, &MainWindow::onChangePage);
    connect(ui->action3, &QAction::toggled, this, &MainWindow::onChangePage);
    connect(ui->action4, &QAction::toggled, this, &MainWindow::onChangePage);

    // Connect splitters moved signal for resize preview
    connect(ui->previewSplitter, &QSplitter::splitterMoved, this, &MainWindow::scalePreview);

	// So the shortcut works without the menu
	this->addAction(ui->actionFullScreen);
}

MainWindow::~MainWindow()
{
	delete m_pageGroup;
    delete m_fm2;
    delete m_fm1;
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
}

// SLOT
void MainWindow::on_previewCheckBox_stateChanged(int state)
{
	if (Qt::Checked == state) {
        updatePreview(m_fm1->currentPath() + "/" + m_fm1->currentFile());
		ui->preview->show();
	} else {
		ui->preview->hide();
	}
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
        switch(idx) {
        case PAGE_IMAGE_VIEW:
            // Save region metadata if changed
            ui->imageViewPage->saveMetadata();
            break;
        case PAGE_2ND_FM:
            //m_secondFm->blockModelSignals(true);
            break;
        case PAGE_METADATA_EDITOR:
            ui->metadataEditPage->saveMetadata();
            break;
        default:
            break;
        }
	}
}

// [private]
void MainWindow::updatePageData(int page)
{
    QString file = m_fm1->currentPath() + "/" + m_fm1->currentFile();

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
    case PAGE_METADATA_EDITOR:
        ui->metadataEditPage->saveMetadata();
        ui->metadataEditPage->setFile(file);
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
    if (m_fm1->isActive()) {
        m_fm1->copy(m_fm2->currentPath());
    } else if (m_fm2->isActive()) {
        m_fm2->copy(m_fm1->currentPath());
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
    if (m_fm1->isActive()) {
        m_fm1->move(m_fm2->currentPath());
    } else if (m_fm2->isActive()) {
        m_fm2->move(m_fm1->currentPath());
	} else {
		qDebug() << "MV Not Active";
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
    if (m_fm1->isActive()) {
        return m_fm1->fileSelection();
    } else if (m_fm2->isActive()) {
        return m_fm2->fileSelection();
	} else {
		return QStringList();
	}
}

// Context menu for file-object entries
void MainWindow::showFmContextMenu(const QPoint &pos)
{
	QMenu contextMenu(tr("Context menu"), this);
    contextMenu.addAction(ui->actionEditSocialMetadata);
    contextMenu.addAction(ui->actionEditKeywords);
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
    if (files.isEmpty()) {
        return;
    }

	// USE DESKTOP SERVICES TO OPEN FILES (xdg-open)!
	// The temporary files are deleted when the application is closed!
	if (!QDesktopServices::openUrl(QUrl::fromLocalFile(files.at(0)))) {
		qDebug() << __PRETTY_FUNCTION__ << "Cannot open Url:";
	}
}

void MainWindow::on_actionStart_Process_triggered()
{
    QString cmd = QString("./test.sh %1").arg(m_fm1->currentPath());
    bool rc = QProcess::startDetached(cmd);

    qDebug() << __func__ << "RC:" << rc;
}

void MainWindow::on_actionPreferences_triggered()
{
}

void MainWindow::on_actionAbout_triggered()
{
    QString txt = QString("KrbkACD v.%1\n\n").arg(KrbkACDVersion::krbkAcdVersionString());
    txt += QString("Qt Lib v.%1\n").arg(QT_VERSION_STR);
    txt += QString("Exiv2  v.%1\n").arg(KrbkACDVersion::exiv2VersionString());
    txt += QString("OpenCv v.%1\n").arg(KrbkACDVersion::openCvVersionString());

    QMessageBox::about(this, "About", txt);
}

void MainWindow::on_actionEditKeywords_triggered()
{
    QStringList files = fileSelection();
    if (files.count() == 0) {
        QMessageBox::warning(this, "Edit Metadata Keywords", "No File Selected!");
    } else {
        KeywordsDialog dlg(files);
        dlg.exec();
    }
}
