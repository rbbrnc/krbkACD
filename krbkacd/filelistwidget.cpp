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
 * Confirm on Copy & Move
 *
 */

/* Widget that shows filesystemmodel and handles navigation
 * in directory tree and basic files operations.
 */
FileListWidget::FileListWidget(QWidget *parent) :
	QListView(parent),
	m_model(new QFileSystemModel(this)),
	m_currentDir(QDir::current())
{
	setModel(m_model);
	setRootIndex(m_model->index(m_currentDir.absolutePath()));

	m_model->setRootPath(m_currentDir.absolutePath());
	m_model->setFilter(QDir::Files | QDir::System | QDir::NoDot | QDir::Dirs);
	m_model->setReadOnly(false);

	connect(this, SIGNAL(activated(QModelIndex)), this, SLOT(handleItemActivation(QModelIndex)));

	setSelectionMode(QAbstractItemView::SingleSelection);

	m_selectionModel = selectionModel();
	connect(m_selectionModel,
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
#if 0
	QString file = m_model->fileInfo(selectedIndexes().first()).filePath();
	::renameFile(file, this);
	setSelectionMode(QAbstractItemView::SingleSelection);
#else
	m_selection = selectedIndexes();

	if (m_selection.isEmpty()) {
		QMessageBox::critical(this, tr("Error"), tr("No file(s) selected"), QMessageBox::Ok);
	} else {
		m_model->setReadOnly(false);
		QStringList files;
		for (int i = 0; i < m_selection.count(); i++) {
			files << m_model->fileInfo(selectedIndexes().at(i)).filePath();
		}
		::renameFiles(files, this);
		m_model->setReadOnly(true);
	}

	setSelectionMode(QAbstractItemView::SingleSelection);
#endif
}

/* SLOT [public]
 *
 * Deletes selected files
 */
void FileListWidget::actionDelete()
{
	m_selection = selectedIndexes();

	if (m_selection.isEmpty()) {
		QMessageBox::critical(this, tr("Error"), tr("No file(s) selected"), QMessageBox::Ok);
	} else {
		QString message = "Delete ";
		int count = m_selection.count();
		if (count == 1) {
			message += m_model->fileName(m_selection.first()) + "?";
		} else {
			message += QString::number(count) + " files?";
		}
		if (QMessageBox::Yes == QMessageBox::warning(this, tr("Delete"), message, QMessageBox::Yes, QMessageBox::No)) {
			m_model->setReadOnly(false);
			m_selection = selectedIndexes();
			//delete files until filelist empty or error occured
			while (!m_selection.isEmpty()) {
				if (m_model->remove(m_selection.first())) {
					m_selection.removeFirst();
				} else {
					QMessageBox::critical(this,
							      tr("Error!"),
							      tr("Deleting file %1 failed").arg(m_model->fileName(m_selection.first())),
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
	return m_currentDir.absolutePath();
}

/*  SLOT [public]
 *
 *  Changes current directory
 *
 * @param path directory to change to
 */
void FileListWidget::changePath(QString path)
{
	m_currentDir.cd(path);
	QString newPath = m_currentDir.absolutePath();

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
	m_selection = selectedIndexes();
	if (m_selection.isEmpty()) {
		return;
	}

	m_model->setReadOnly(false);

	// Copy files until filelist is empty or error occured
	while (!m_selection.isEmpty()) {
		if (!::copyFile(destPath, m_model->fileInfo(m_selection.first()).absoluteFilePath(), this)) {
			break;
		}
		m_selection.removeFirst();
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
	m_selection = selectedIndexes();
	if (m_selection.isEmpty()) {
		return;
	}

	m_model->setReadOnly(false);

	// Move files until filelist is empty or error occured
	while (!m_selection.isEmpty()) {
		if (!::moveFile(destPath, m_model->fileInfo(m_selection.first()).absoluteFilePath(), this)) {
			break;
		}
		m_selection.removeFirst();
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

QStringList FileListWidget::selectedFiles(QDir::Filter filters)
{
	QStringList list;

	m_selection = selectedIndexes();

	m_model->setReadOnly(false);

	QFlags<QDir::Filter> fflags(filters);

	while (!m_selection.isEmpty()) {
		QFileInfo fi = m_model->fileInfo(m_selection.first());

		if (fflags.testFlag(QDir::Dirs)) {
			if (fi.isDir()) {
				if ((fi.fileName() != ".") && (fi.fileName() != "..")) {
					list.append(fi.absoluteFilePath());
				}
			}
			m_selection.removeFirst();
			continue;
		}

		if (fflags.testFlag(QDir::Files)) {
			if (fi.isFile()) {
				list.append(m_model->fileInfo(m_selection.first()).absoluteFilePath());
			}
			m_selection.removeFirst();
		}
	}

	m_model->setReadOnly(true);
	clearSelection();
	setSelectionMode(QAbstractItemView::SingleSelection);
	return list;
}
