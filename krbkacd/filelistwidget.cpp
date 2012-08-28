#include <QMessageBox>
#include <QInputDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QProcess>

//#include <QDebug>

#include "filelistwidget.h"
#include "file_utils.h"

/* TODO
 *
 * Add Warning on delete directory not empty!
 * Batch rename
 * Confirm on Copy & Move
 *
 */

/* Widget that shows filesystemmodel and handles navigation
 * in directory tree and basic files operations.
 */
FileListWidget::FileListWidget(QWidget *parent) :
	QListView(parent),
	m_model(new QFileSystemModel(this)),
	//currentDir(QDir::homePath())
	currentDir(QDir::current())
{
	setModel(m_model);
	setRootIndex(m_model->index(currentDir.absolutePath()));

	m_model->setRootPath(currentDir.absolutePath());
	m_model->setFilter(QDir::Files | QDir::System | QDir::NoDot | QDir::Dirs);
	m_model->setReadOnly(false);

	connect(this, SIGNAL(activated(QModelIndex)), this, SLOT(handleItemActivation(QModelIndex)));

	setSelectionMode(QAbstractItemView::SingleSelection);

	selModel = selectionModel();
	connect(selModel,
                SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
                this,
                SLOT(fileSelect(const QModelIndex &, const QModelIndex &)));
}

/* SLOT [public]
 *
 * Switches view mode
 * @param iconmode true shows iconview, false shows listview
 */
void FileListWidget::actionSwitchMode(bool iconmode)
{
	if (iconmode) {
		setViewMode(QListView::IconMode);
		setWordWrap(true);
		setGridSize(QSize(80,80));
	} else {
		setViewMode(QListView::ListMode);
		setWordWrap(false);
		setGridSize(QSize());
	}
}

/* SLOT [public]
 *
 * Switches show hidden
 *
 * @param show true shows hidden files
 */
void FileListWidget::actionShowHidden(bool show)
{
	if (show) {
		m_model->setFilter(m_model->filter() | QDir::Hidden);
	} else {
		m_model->setFilter(m_model->filter() &~ QDir::Hidden);
	}

	clearSelection();
}

/* SLOT [public]
 *
 * Rename selected file
 */
void FileListWidget::actionRename()
{
	QString file = m_model->fileInfo(selectedIndexes().first()).filePath();
	::renameFile(file, this);
	setSelectionMode(QAbstractItemView::SingleSelection);
}

/* SLOT [public]
 *
 * Deletes selected files
 */
void FileListWidget::actionDelete()
{
	selectedFiles = selectedIndexes();

	if (selectedFiles.isEmpty()) {
		QMessageBox::critical(this, tr("Error"), tr("No file(s) selected"), QMessageBox::Ok);
	} else {
		QString message = "Delete ";
		int count = selectedFiles.count();
		if (count == 1) {
			message += m_model->fileName(selectedFiles.first()) + "?";
		} else {
			message += QString::number(count) + " files?";
		}
		if (QMessageBox::Yes == QMessageBox::warning(this, tr("Delete"), message, QMessageBox::Yes, QMessageBox::No)) {
			m_model->setReadOnly(false);
			selectedFiles = selectedIndexes();
			//delete files until filelist empty or error occured
			while (!selectedFiles.isEmpty()) {
				if (m_model->remove(selectedFiles.first())) {
					selectedFiles.removeFirst();
				} else {
					QMessageBox::critical(this,
							      tr("Error!"),
							      tr("Deleting file %1 failed").arg(m_model->fileName(selectedFiles.first())),
							      QMessageBox::Ok);
					break;
				}
			}
			m_model->setReadOnly(true);
			clearSelection();
		}
	}

	setSelectionMode(QAbstractItemView::SingleSelection);
}

/**  @return Current directory shown  */
QString FileListWidget::getPath()
{
	return currentDir.absolutePath();
}

/*  SLOT [public]
 *
 *  Changes current directory
 *
 * @param path directory to change to
 */
void FileListWidget::changePath(QString path)
{
	currentDir.cd(path);
	QString newPath = currentDir.absolutePath();

	m_model->setRootPath(newPath);
	clearSelection();
	setRootIndex(m_model->index(newPath));
	emit pathChanged(newPath);
	setSelectionMode(QAbstractItemView::SingleSelection);
}

/*  SLOT [private] */
void FileListWidget::fileSelect(const QModelIndex &current, const QModelIndex &/*previous*/)
{
        QFileInfo fileInfo = m_model->fileInfo(current);
	emit selectionChanged(fileInfo.absoluteFilePath());
}

/*  SLOT [private] */
void FileListWidget::handleItemActivation(QModelIndex index)
{
	if (selectionMode() == QAbstractItemView::SingleSelection) {
		QFileInfo file = m_model->fileInfo(index);
		if (file.isDir()) {
			changePath(file.absoluteFilePath());
		}
#if 0 // Abilita esecuzione o apertura
		else if(file.isExecutable()) {
			// Make process
			QProcess::startDetached(file.absoluteFilePath());
		} else {
			QDesktopServices::openUrl(QUrl::fromLocalFile(file.absoluteFilePath()));
		}
#endif
	}
	setSelectionMode(QAbstractItemView::SingleSelection);
}

void FileListWidget::actionCopyFile(const QString destPath)
{
	selectedFiles = selectedIndexes();
	if (selectedFiles.isEmpty()) {
		return;
	}

	m_model->setReadOnly(false);

	// Copy files until filelist is empty or error occured
	while (!selectedFiles.isEmpty()) {
		if (!::copyFile(destPath, m_model->fileInfo(selectedFiles.first()).absoluteFilePath(), this)) {
			break;
		}
		selectedFiles.removeFirst();
	}

	m_model->setReadOnly(true);
	clearSelection();
	setSelectionMode(QAbstractItemView::SingleSelection);
}

/* SLOT [public]
 *
 * Move files that were selected when called
 */
void FileListWidget::actionMoveFile(const QString destPath)
{
	selectedFiles = selectedIndexes();
	if (selectedFiles.isEmpty())
		return;

	m_model->setReadOnly(false);

	// Move files until filelist is empty or error occured
	while (!selectedFiles.isEmpty()) {
		if (!::moveFile(destPath, m_model->fileInfo(selectedFiles.first()).absoluteFilePath(), this)) {
			break;
		}
		selectedFiles.removeFirst();
	}

	m_model->setReadOnly(true);
	clearSelection();
	setSelectionMode(QAbstractItemView::SingleSelection);
}

/* SLOT [public]
 *
 * Create New Directory
 */
void FileListWidget::actionMkDir(const QString destPath)
{
	::makeDir(destPath, this);
}
