#include "FileManager.h"
#include "ui_FileManager.h"

//#include <QDebug>
#include <QMessageBox>
#include <QInputDialog>

#include "RenameDialog.h"
#include "BatchRenameDialog.h"
#include "CopyMoveDialog.h"
#include "QMagic.h"
#include "QExiv2.h"
#include "MetadataTreeModel.h"

FileManager::FileManager(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::FileManager)
{
	m_metadataModel = 0;

	ui->setupUi(this);

	m_model = new QFileSystemModel();

	m_currentIndex = m_model->setRootPath(QDir::current().absolutePath());

	m_model->setFilter(QDir::Files | QDir::System | QDir::NoDot | QDir::Dirs);
	m_model->setReadOnly(true);

	ui->listView->setModel(m_model);
	ui->listView->setRootIndex(m_currentIndex);
	ui->listView->setSelectionMode(QAbstractItemView::SingleSelection);

	m_selectionModel = ui->listView->selectionModel();

	connect(m_selectionModel,
		SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
		this,
		SLOT(fileSelect(const QModelIndex &, const QModelIndex &)));

	connect(ui->listView, SIGNAL(activated(QModelIndex)),
		this, SLOT(handleItemActivation(QModelIndex)));

	// Connect splitters moved signal for resize preview
	connect(ui->infoSplitter, SIGNAL(splitterMoved(int, int)),
		this, SLOT(zoomPreview(int, int)));
	connect(ui->mainSplitter, SIGNAL(splitterMoved(int, int)),
		this, SLOT(zoomPreview(int, int)));

	connect(ui->infoToolBox, SIGNAL(currentChanged(int)),
		this, SLOT(updateInfoToolBox(int)));

	ui->pathLabel->setText(m_model->rootPath());

	// Defaut for info box
	ui->previewCheckBox->setChecked(false);
	ui->previewLabel->hide();

	connect(ui->previewCheckBox, SIGNAL(toggled(bool)), this, SLOT(showInfo(bool)));
}

FileManager::~FileManager()
{
	if (m_metadataModel) {
		delete m_metadataModel;
		m_metadataModel = 0;
	}

	delete m_model;
	delete ui;
}

/* @return Current directory shown  */
QString FileManager::currentPath() const
{
	return m_currentDir.absolutePath();
}

/* @return last selected file name  */
QString FileManager::currentFile() const
{
	return m_currentFileName;
}

// [public]
bool FileManager::isActive() const
{
	return ui->listView->hasFocus();
}

/*  SLOT [private] */
void FileManager::fileSelect(const QModelIndex &current, const QModelIndex &/*previous*/)
{
	m_currentFileName = m_model->fileName(current);
	updateInfo();
	emit currentChanged(m_currentFileName);
}

/*  SLOT [private] */
void FileManager::handleItemActivation(QModelIndex index)
{
	QListView *lw = ui->listView; //static_cast<QListView *>(sender());
	if (lw->selectionMode() == QAbstractItemView::SingleSelection) {
		if (m_model->isDir(index)) {
			// Change Path
		        m_currentDir.cd(m_model->filePath(index));
			m_currentIndex = m_model->setRootPath(m_currentDir.absolutePath());

			updateInfo();

		        lw->clearSelection();
			lw->setRootIndex(m_currentIndex);
			lw->setSelectionMode(QAbstractItemView::SingleSelection);
		        // Set view on firstRow
			lw->setCurrentIndex(m_currentIndex.child(0, 0));
		}
	}
	lw->setSelectionMode(QAbstractItemView::SingleSelection);
}

void FileManager::previous()
{
	int current_row = ui->listView->currentIndex().row();
	QModelIndex mi = m_currentIndex.child(--current_row, 0);

	if (mi.isValid()) {
		ui->listView->setCurrentIndex(mi);
		updateInfo();
	}
}

void FileManager::next()
{
	int current_row = ui->listView->currentIndex().row();

	QModelIndex mi = m_currentIndex.child(++current_row, 0);
	if (mi.isValid()) {
		ui->listView->setCurrentIndex(mi);
		updateInfo();
	}
}

void FileManager::updateInfoToolBox(int /*index*/)
{
	updateInfo();
}

// [SLOT public]
//
// Switches view mode
// @param iconmode true shows iconview, false shows listview
//
void FileManager::iconMode(bool enable)
{
	if (enable) {
		ui->listView->setViewMode(QListView::IconMode);
                ui->listView->setWordWrap(true);
		ui->listView->setGridSize(QSize(80,80));
	} else {
		ui->listView->setViewMode(QListView::ListMode);
		ui->listView->setWordWrap(false);
		ui->listView->setGridSize(QSize());
	}
}

// [SLOT public]
void FileManager::showHidden(bool show)
{
        if (show) {
                m_model->setFilter(m_model->filter() | QDir::Hidden);
        } else {
                m_model->setFilter(m_model->filter() &~ QDir::Hidden);
        }
}

void FileManager::updateGeneralInfo(const QModelIndex &index)
{
	QString file = m_model->filePath(index);

	ui->pathLabel->setText(m_model->rootPath());
	ui->fileLabel->setText(m_model->fileName(index));

	//ui->kindLabel->setText(QMagic::mimeDescription(file));
	ui->kindLabel->setText(QMagic::mimeType(file));
	ui->sizeLabel->setText(QString("%1 bytes").arg(m_model->size(index)));
	ui->modifiedLabel->setText(m_model->lastModified(index).toString());

	QFileInfo fi(file);
	ui->createdLabel->setText(fi.created().toString());
	ui->ownerLabel->setText(fi.owner());
	ui->groupLabel->setText(fi.group());

	QFile::Permissions fp = fi.permissions();
	QString perm;
	perm  = (fp & QFile::ReadOwner)  ? 'r' : '-';
	perm += (fp & QFile::WriteOwner) ? 'w' : '-';
	perm += (fp & QFile::ExeOwner)   ? "x " : "- ";
	perm += (fp & QFile::ReadGroup)  ? 'r' : '-';
	perm += (fp & QFile::WriteGroup) ? 'w' : '-';
	perm += (fp & QFile::ExeGroup)   ? "x " : "- ";
	perm += (fp & QFile::ReadOther)  ? 'r' : '-';
	perm += (fp & QFile::WriteOther) ? 'w' : '-';
	perm += (fp & QFile::ExeOther)   ? "x " : "- ";
	ui->permissionsLabel->setText(perm);

	if (QMagic::mimeTypeIsImage(file)) {
		// Using QImageReader for get image dimension.
		QImageReader ir(file);
		ui->dimensionLabel->setText(QString("%1 x %2").arg(ir.size().width()).arg(ir.size().height()));
	} else {
		ui->dimensionLabel->setText("-");
	}
}

void FileManager::updateMoreInfo(const QModelIndex &index)
{
	QString file = m_model->filePath(index);
	if (QMagic::mimeTypeIsImage(file)) {
		QImageReader ir(file);
		// XXX: QImage reader supports basic metadata on some image format!
		if (ir.supportsOption(QImageIOHandler::Description)) {
#if 0
			qDebug() << __PRETTY_FUNCTION__
				 << "IR Text Keys:" << ir.textKeys()
				 << "IR Text XML:" << ir.text("XML"/* const QString & key*/)
				 << "IR Text Desc:" << ir.text("Description"/* const QString & key*/);
#endif
			ui->commentLabel->setText(ir.text("Description"));
			ui->commentLabel->setVisible(true);
		}
	} else {
		ui->commentLabel->setVisible(false);
	}
}

void FileManager::updateMetadataInfo(const QModelIndex &index)
{
	QString file = m_model->filePath(index);
	QString mimeType = QMagic::mimeType(file);
	bool isImage = mimeType.contains("image");
	bool isVideo = mimeType.contains("video");

	if (!isImage && !isVideo) {
		return;
	}

	QExiv2 metadata;

	metadata.load(file);
	if (!metadata.isValid()) {
		return;
	}

	// Setup model data.
	if (m_metadataModel) {
		delete m_metadataModel;
		m_metadataModel = 0;
	}

	m_metadataModel = new MetadataTreeModel(&metadata);
	ui->treeView->setModel(m_metadataModel);
	ui->treeView->hideColumn(7); // hide key column
}

void FileManager::updatePreview(const QModelIndex &index)
{
	QString file = m_model->filePath(index);
	if (!QMagic::mimeTypeIsImage(file)) {
		m_preview = QPixmap();
		ui->previewLabel->setPixmap(m_preview);
	} else {
		m_preview.load(file);
		if ((m_preview.width() > ui->previewLabel->width()) || (m_preview.height() > ui->previewLabel->height())) {
			// Scale pixmap to fit into preview widget
			QPixmap pix;
			pix = m_preview.scaled(ui->previewLabel->width(), ui->previewLabel->height(), Qt::KeepAspectRatio, Qt::FastTransformation);
			ui->previewLabel->setPixmap(pix);
		} else {
			ui->previewLabel->setPixmap(m_preview);
		}
	}
}

void FileManager::zoomPreview(int, int)
{
	if ((m_preview.width() > ui->previewLabel->width()) || (m_preview.height() > ui->previewLabel->height())) {
		QPixmap pix;
		pix = m_preview.scaled(ui->previewLabel->width(), ui->previewLabel->height(), Qt::KeepAspectRatio, Qt::FastTransformation);
		ui->previewLabel->setPixmap(pix);
	}
}

void FileManager::updateInfo()
{
	if (!ui->infoContainerWidget->isVisible()) {
		return;
	}

	QModelIndex mi = m_currentIndex.child(ui->listView->currentIndex().row(), 0);
	if (!mi.isValid()) {
		return;
	}

	switch (ui->infoToolBox->currentIndex()) {
	case 0:
		updateGeneralInfo(mi);
		break;
	case 1:
		updateMoreInfo(mi);
		break;
	case 2:
		updateMetadataInfo(mi);
		break;
	default:
		break;
	}

	if (ui->previewCheckBox->isChecked()) {
		updatePreview(mi);
	}
}

// [SLOT public]
void FileManager::showInfo(bool show)
{
	QCheckBox *cb = static_cast<QCheckBox *>(sender());
	QWidget   *container;

	if (cb == ui->previewCheckBox) {
		container = static_cast<QWidget *>(ui->previewLabel);
		QModelIndex mi = m_currentIndex.child(ui->listView->currentIndex().row(), 0);
		if (mi.isValid() && show) {
			updatePreview(mi);
		}
		container->setVisible(show);
	} else {
		container = ui->infoContainerWidget;
		container->setVisible(show);
		if (show) {
			updateInfo();
		}
	}
}

// [EVENT]
void FileManager::keyPressEvent(QKeyEvent *event)
{
	if ((event->key() == Qt::Key_Control) || (event->key() == Qt::Key_Shift)) {
		ui->listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
	} else {
		QWidget::keyPressEvent(event);
	}
}

// [EVENT]
void FileManager::keyReleaseEvent(QKeyEvent *event)
{
	if ((event->key() == Qt::Key_Control) || (event->key() == Qt::Key_Shift)) {
		ui->listView->setSelectionMode(QAbstractItemView::SingleSelection);
	} else {
		QWidget::keyPressEvent(event);
	}
}

// [SLOT private]
void FileManager::mkDir()
{
	QString dirName = QInputDialog::getText(this,
				QObject::tr("Create a new directory"),
				QObject::tr("Enter directory name: "),
				QLineEdit::Normal);
	// Remove whitespaces
	dirName = dirName.simplified();

	if (dirName.isEmpty()) {
		return;
	}

	QModelIndex index = m_model->mkdir(m_currentIndex, dirName);
	if (!index.isValid()) {
		QMessageBox::critical(this,
			QObject::tr("Error"),
			QObject::tr("Creating dir '%1' failed").arg(dirName),
			QMessageBox::Abort);
	}
}

// [SLOT public]
void FileManager::remove()
{
	QModelIndexList m_selection = m_selectionModel->selectedIndexes();
	if (m_selection.isEmpty()) {
		QMessageBox::critical(this, tr("Error"), tr("No file(s) selected"), QMessageBox::Ok);
		return;
	}

	QString msg;
	int count = m_selection.count();
	if (count == 1) {
		QModelIndex index = ui->listView->currentIndex();
		if (m_model->isDir(index)) {
			msg = tr("Delete Directory %1?").arg(m_model->fileName(index));
		} else {
			msg = tr("Delete File %1?").arg(m_model->fileName(index));
		}
	} else {
		msg = tr("Delete %1 Files?").arg(count);
	}

	if (QMessageBox::No == QMessageBox::warning(this, "Delete", msg, QMessageBox::Yes, QMessageBox::No)) {
		return;
	}

	for (int i = 0; i < count; i++) {
		if (!m_model->remove(m_selection.at(i))) {
			QMessageBox::critical(this, QObject::tr("Error"),
				tr("Cannot Remove '%1'").arg(m_model->filePath(m_selection.at(i))),
				QMessageBox::Abort);
			return;
		}
	}
}

// [SLOT public]
void FileManager::rename()
{
	QModelIndexList m_selection = m_selectionModel->selectedIndexes();

	if (m_selection.isEmpty()) {
		QMessageBox::critical(this, tr("Error"), tr("No file(s) selected"), QMessageBox::Ok);
		return;
	}

	m_model->setReadOnly(false);

	if (m_selection.count() == 1) {
		// Single file
		RenameDialog dlg(m_model->fileName(m_selection.first()));
		if (dlg.exec() == QDialog::Accepted) {
			if (m_model->setData(m_selection.first(), dlg.newFileName())) {
				ui->listView->update(m_selection.first());
			}
		}
	} else {
		// Multi files
		QStringList files;
		for (int i = 0; i < m_selection.count(); i++) {
			files << m_model->fileName(m_selection.at(i));
			//qDebug() << m_model->filePath(m_selection.at(i));
		}

		BatchRenameDialog dlg(files);
		if (dlg.exec() == QDialog::Accepted) {
			QStringList filesOut = dlg.newFileNames();
			for (int i = 0; i < m_selection.count(); i++) {
				//qDebug() << m_model->fileName(m_selection.at(i)) << "--" << filesOut.at(i);
				if (m_model->setData(m_selection.at(i), filesOut.at(i))) {
					ui->listView->update(m_selection.at(i));
				}
			}
		}
	}

	m_model->setReadOnly(true);
	ui->listView->setSelectionMode(QAbstractItemView::SingleSelection);
}

void FileManager::copy(const QString &destPath)
{
	m_selection = m_selectionModel->selectedIndexes();
	if (m_selection.isEmpty()) {
		QMessageBox::critical(this, tr("Error"), tr("No file(s) selected"), QMessageBox::Ok);
		return;
	}

	if (destPath.isNull() || destPath.isEmpty()) {
		QMessageBox::critical(this, tr("Error"), tr("Invalid destPath!"), QMessageBox::Ok);
		return;
	}

	int dc;
	if (m_selection.count() == 1) {
		CopyMoveDialog dlg(m_model->fileName(m_selection.first()), destPath);
		dlg.setWindowTitle("Copy File");
		dc = dlg.exec();
	} else {
		CopyMoveDialog dlg(m_model->fileName(m_selection.first()), destPath);
		dlg.setWindowTitle("Copy File(s)");
		dc = dlg.exec();
	}

	if (QDialog::Accepted != dc) {
		return;
	}

	m_model->setReadOnly(false);

	QString dest;
	// Copy files until filelist is empty or error occured
	while (!m_selection.isEmpty()) {
		dest = destPath + "/" + m_model->fileName(m_selection.first());
		if (QFile::exists(dest)) {
			QMessageBox::critical(this, tr("Copy Error"), tr("File '%1' already exists").arg(dest), QMessageBox::Abort);
			m_model->setReadOnly(true);
			return;
		}
		if (!QFile::copy(m_model->filePath(m_selection.first()), dest)) {
			QMessageBox::critical(this, tr("Copy Error"),
				tr("Copying file '%1' failed").arg(m_model->fileName(m_selection.first())),
				QMessageBox::Abort);

			m_model->setReadOnly(true);
			return;
		}
		m_selection.removeFirst();
	}

	m_model->setReadOnly(true);
	ui->listView->clearSelection();
	ui->listView->setSelectionMode(QAbstractItemView::SingleSelection);
}

void FileManager::move(const QString &destPath)
{
	m_selection = m_selectionModel->selectedIndexes();
	if (m_selection.isEmpty()) {
		QMessageBox::critical(this, tr("Error"), tr("No file(s) selected"), QMessageBox::Ok);
		return;
	}

	if (destPath.isNull() || destPath.isEmpty()) {
		QMessageBox::critical(this, tr("Error"), tr("Invalid destPath!"), QMessageBox::Ok);
		return;
	}

	int dc;

	if (m_selection.count() == 1) {
		CopyMoveDialog dlg(m_model->fileName(m_selection.first()), destPath);
		dlg.setWindowTitle("Move File");
		dc = dlg.exec();
	} else {
		CopyMoveDialog dlg(m_model->fileName(m_selection.first()), destPath);
		dlg.setWindowTitle("Move File(s)");
		dc = dlg.exec();
	}

	if (QDialog::Accepted != dc) {
		return;
	}

	m_model->setReadOnly(false);

	QString dest;
	// Move files until filelist is empty or error occured
	while (!m_selection.isEmpty()) {
		dest = destPath + "/" + m_model->fileName(m_selection.first());
		if (QFile::exists(dest)) {
			QMessageBox::critical(this, tr("Move Error"), tr("File '%1' already exists").arg(dest), QMessageBox::Abort);
			m_model->setReadOnly(true);
			return;
		}
		if (!QFile::rename(m_model->filePath(m_selection.first()), dest)) {
			QMessageBox::critical(this, tr("Move Error"),
				tr("Moving file '%1' failed").arg(m_model->fileName(m_selection.first())),
				QMessageBox::Abort);

			m_model->setReadOnly(true);
			return;
		}
		m_selection.removeFirst();
	}
	m_model->setReadOnly(true);
	ui->listView->clearSelection();
	ui->listView->setSelectionMode(QAbstractItemView::SingleSelection);
}
