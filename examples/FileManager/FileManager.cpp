#include "FileManager.h"
#include "ui_FileManager.h"

#include <QDebug>
#include <QMessageBox>
#include <QInputDialog>

#include "RenameDialog.h"
#include "BatchRenameDialog.h"
#include "CopyMoveDialog.h"
#include "QMagic.h"
#include "QExiv2.h"

FileManager::FileManager(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::FileManager)
{
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

//	connect(m_model, SIGNAL(rootPathChanged(const QString &)),
//		this, SLOT(currentPathChanged(const QString &)));

	ui->pathLabel->setText(m_model->rootPath());

	// Defaut for info box
	ui->infoCheckBox->setChecked(true);
	ui->moreInfoCheckBox->setChecked(false);
	ui->moreInfoWidget->hide();
	ui->previewCheckBox->setChecked(false);
	ui->previewLabel->hide();
//	ui->infoContainerWidget;

	connect(ui->infoCheckBox, SIGNAL(toggled(bool)), this, SLOT(showInfo(bool)));
	connect(ui->moreInfoCheckBox, SIGNAL(toggled(bool)), this, SLOT(showInfo(bool)));
	connect(ui->previewCheckBox, SIGNAL(toggled(bool)), this, SLOT(showInfo(bool)));
}

FileManager::~FileManager()
{
	delete m_model;
	delete ui;
}

#if 0
void FileManager::currentPathChanged(const QString &newPath)
{
	qDebug() << newPath;
}
#endif

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

/*
void FileManager::on_pushButton_clicked()
{
	// index = first row
	QModelIndex index = m_currentIndex.child(0, 0);
	do {
		if (index.isValid())
			qDebug() << m_model->fileName(index);
		else
			break;

		index = m_fs->nextRow(index);
	} while (index.isValid());
}
*/

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

void FileManager::updateInfo()
{
	if (!ui->infoContainerWidget->isVisible()) {
		return;
	}

	QModelIndex mi = m_currentIndex.child(ui->listView->currentIndex().row(), 0);
	QString file   = m_model->filePath(mi);

	ui->pathLabel->setText(m_model->rootPath());
	ui->fileLabel->setText(m_model->fileName(mi));

	if (file.isEmpty() || file.isNull()) {
		if (ui->infoWidget->isVisible()) {
			ui->kindLabel->setText("");
			ui->sizeLabel->setText("");
			ui->modifiedLabel->setText("");
			ui->createdLabel->setText("");
			ui->ownerLabel->setText("");
			ui->groupLabel->setText("");
			ui->permissionsLabel->setText("");
		}
		if (ui->moreInfoWidget->isVisible()) {
			ui->dimensionLabel->hide();
			ui->exifLabel->setText("-");
			ui->iptcLabel->setText("-");
			ui->xmpLabel->setText("-");
			ui->commentLabel->setText("-");
		}

		if (ui->previewLabel->isVisible()) {
			ui->previewLabel->setPixmap(QPixmap());
		}
		return;
	}

//	qDebug() << __PRETTY_FUNCTION__ << m_model->filePath(mi);
	QString mimeType = QMagic::mimeType(file);


	if (ui->infoWidget->isVisible()) {
		//ui->kindLabel->setText(QMagic::mimeDescription(file));
		ui->kindLabel->setText(mimeType);
		ui->sizeLabel->setText(QString("%1 bytes").arg(m_model->size(mi)));
		ui->modifiedLabel->setText(m_model->lastModified(mi).toString());

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
	}

	bool isImage = mimeType.contains("image");
	if (ui->moreInfoWidget->isVisible()) {
		QExiv2 metadata;
		if (isImage) {
			QImageReader ir(file);
			ui->dimensionLabel->setText(QString("%1 x %2").arg(ir.size().width()).arg(ir.size().height()));
			ui->dimensionLabel->show();
			 if (ir.supportsOption(QImageIOHandler::Description)) {
				qDebug() << ir.textKeys();
				qDebug() << ir.text("XML"/* const QString & key*/);
				qDebug() << ir.text("Description"/* const QString & key*/);
			}

			qDebug() << "A";
			metadata.load(file);
			qDebug() << "B";
		} else {
			ui->dimensionLabel->hide();
		}

		if (metadata.isValid()) {
			ui->exifLabel->setText((metadata.hasExif()) ? "Yes" : "No");
			ui->iptcLabel->setText((metadata.hasIptc()) ? "Yes" : "No");
			ui->xmpLabel->setText((metadata.hasXmp()) ? "Yes" : "No");
			ui->commentLabel->setText((metadata.hasComment()) ? "Yes" : "No");
		} else {
			ui->exifLabel->setText("-");
			ui->iptcLabel->setText("-");
			ui->xmpLabel->setText("-");
			ui->commentLabel->setText("-");
		}
	}

	if (ui->previewLabel->isVisible()) {
		QPixmap pix;
		if (isImage) {
			//QMimeData md;
			//QMagic::mimeData(md, file);
			//QImage image = qvariant_cast<QImage>(md.imageData());
			//image = image.scaled(128, 128, Qt::KeepAspectRatio, Qt::FastTransformation);
			//ui->previewLabel->setPixmap(QPixmap::fromImage(image));
			pix = QPixmap(file).scaled(128, 128, Qt::KeepAspectRatio, Qt::FastTransformation);
		}
		ui->previewLabel->setPixmap(pix);
	}
}

// [SLOT public]
void FileManager::showInfo(bool show)
{
	QCheckBox *cb = static_cast<QCheckBox *>(sender());
	QWidget   *container;

	if (cb == ui->infoCheckBox) {
		container = ui->infoWidget;
	} else if (cb == ui->moreInfoCheckBox) {
		container = ui->moreInfoWidget;
	} else if (cb == ui->previewCheckBox) {
		container = static_cast<QWidget *>(ui->previewLabel);
	} else {
		container = ui->infoContainerWidget;
	}

	if (show) {
		container->show();
		updateInfo();
	} else {
		container->hide();
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

	QModelIndex index = ui->listView->currentIndex();
	QString msg;

	if (m_model->isDir(index)) {
		msg = tr("Delete Directory %1?").arg(m_model->fileName(index));
	} else {
		msg = tr("Delete File %1?").arg(m_model->fileName(index));
	}

	if (QMessageBox::No == QMessageBox::warning(this, "Delete", msg, QMessageBox::Yes, QMessageBox::No)) {
		return;
	}

	if (m_model->remove(index)) {
		return;
	}

	QMessageBox::critical(this, QObject::tr("Error"),
		tr("Cannot Remove '%1'").arg(m_model->filePath(index)),
		QMessageBox::Abort);
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
				qDebug() << m_model->fileName(m_selection.at(i)) << "--" << filesOut.at(i);
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


