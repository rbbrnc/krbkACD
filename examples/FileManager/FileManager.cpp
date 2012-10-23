#include "FileManager.h"
#include "ui_FileManager.h"

#include <QDebug>
#include <QMessageBox>
#include <QInputDialog>

#include "RenameDialog.h"
#include "BatchRenameDialog.h"

FileManager::FileManager(const QString &path, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::FileManager)
{
	ui->setupUi(this);

	m_model = new QFileSystemModel();
	if (path.isNull() || path.isEmpty()) {
		m_currentIndex = m_model->setRootPath(QDir::current().absolutePath());
	} else {
		m_currentIndex = m_model->setRootPath(path);
	}

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
}

FileManager::~FileManager()
{
	delete m_model;
	delete ui;
}

/* @return Current directory shown  */
QString FileManager::currentPath() const
{
	return m_currentDir.absolutePath();
}


/*  SLOT [private] */
void FileManager::fileSelect(const QModelIndex &current, const QModelIndex &/*previous*/)
{
	ui->label->setText(m_model->fileName(current));
}

/*  SLOT [private] */
void FileManager::handleItemActivation(QModelIndex index)
{
	QListView *lw = ui->listView; //static_cast<QListView *>(sender());
	if (lw->selectionMode() == QAbstractItemView::SingleSelection) {
		QFileInfo file = m_model->fileInfo(index);
		if (file.isDir()) {
			// Change Path
		        m_currentDir.cd(file.absoluteFilePath());
			m_currentIndex = m_model->setRootPath(m_currentDir.absolutePath());

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
		ui->label->setText(m_model->fileName(mi));
	}
}

void FileManager::next()
{
	int current_row = ui->listView->currentIndex().row();

	QModelIndex mi = m_currentIndex.child(++current_row, 0);
	if (mi.isValid()) {
		ui->listView->setCurrentIndex(mi);
		ui->label->setText(m_model->fileName(mi));
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
			files << m_model->filePath(m_selection.at(i));
			//qDebug() << m_model->filePath(m_selection.at(i));
		}

		BatchRenameDialog dlg(files);
		if (dlg.exec() == QDialog::Accepted) {
			QStringList filesOut = dlg.newFileNames();
		}
	}

	m_model->setReadOnly(true);
	ui->listView->setSelectionMode(QAbstractItemView::SingleSelection);
}

void FileManager::copy(const QString &destPath)
{
	m_selection = m_selectionModel->selectedIndexes();
	if (m_selection.isEmpty()) {
		return;
	}

	m_model->setReadOnly(false);

	// Copy files until filelist is empty or error occured
	while (!m_selection.isEmpty()) {
#if 0
		if (!::copyFile(destPath, m_model->fileInfo(m_selection.first()).absoluteFilePath(), this)) {
			break;
		}
#endif
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
		return;
	}

	m_model->setReadOnly(false);

	// Move files until filelist is empty or error occured
	while (!m_selection.isEmpty()) {
#if 0
		if (!::moveFile(destPath, m_model->filePath(m_selection.first()), this)) {
			break;
		}
#endif
		qDebug() << m_model->filePath(m_selection.first());

		m_selection.removeFirst();
	}
	m_model->setReadOnly(true);
	ui->listView->clearSelection();
	ui->listView->setSelectionMode(QAbstractItemView::SingleSelection);
}


