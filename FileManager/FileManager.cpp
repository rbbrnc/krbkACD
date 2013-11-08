#include <QInputDialog>
#include <QMessageBox>

#include <QDebug>

#include "FileManager.h"
#include "RenameDialog.h"
#include "BatchRenameDialog.h"
#include "CopyMoveDialog.h"

FileManager::FileManager(QListView *listView, QWidget *parent)
	: m_view(listView),
	  m_parent(parent)
{
	m_model = new QFileSystemModel();

	m_currentIndex = m_model->setRootPath(QDir::current().absolutePath());

	m_model->setFilter(QDir::Files | QDir::System | QDir::NoDot | QDir::Dirs);
	m_model->setReadOnly(true);

	m_view->setLayoutMode(QListView::Batched);

	m_view->setModel(m_model);
	m_view->setRootIndex(m_currentIndex);
	m_view->setSelectionMode(QAbstractItemView::ExtendedSelection);

	m_selectionModel = m_view->selectionModel();

	connect(m_selectionModel, SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
		this, SLOT(fileSelect(const QModelIndex &, const QModelIndex &)));

	connect(m_view, SIGNAL(activated(QModelIndex)),
		this, SLOT(handleItemActivation(QModelIndex)));

	connect(m_model, SIGNAL(directoryLoaded(QString)),
		 this,   SLOT(onDirectoryLoaded(QString)));


}

void FileManager::onDirectoryLoaded ( const QString & path )
{
	qDebug() << this << "Dir loaded";
}


void FileManager::blockModelSignals(bool block)
{
//	m_selectionModel->blockSignals(!enable);
	m_model->blockSignals(block);
//	m_view->blockSignals(!enable);
}

FileManager::~FileManager()
{
	delete m_model;
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

QStringList FileManager::fileSelection() const
{
	QStringList files;
	QModelIndexList sel = m_selectionModel->selectedIndexes();
	for (int i = 0; i < sel.count(); i++) {
		if (!m_model->isDir(sel.at(i))) {
			files << m_currentDir.absolutePath() + '/' + m_model->fileName(sel.at(i));
		}
	}
	return files;
}

// [public]
bool FileManager::isActive() const
{
	return m_view->hasFocus();
}

/*  SLOT [private] */
void FileManager::fileSelect(const QModelIndex &current, const QModelIndex &/*previous*/)
{
	m_currentFileName = m_model->fileName(current);
//	qDebug() << __PRETTY_FUNCTION__ << m_currentFileName;
	emit currentChanged();
}

// [SLOT public]
void FileManager::deleteSelectedFiles()
{
	QModelIndexList m_selection = m_selectionModel->selectedIndexes();
	if (m_selection.isEmpty()) {
		QMessageBox::critical(m_parent, tr("Error"),
				tr("No file(s) selected"), QMessageBox::Ok);
		return;
	}

	QString msg;
	int count = m_selection.count();
	if (count == 1) {
		QModelIndex index = m_view->currentIndex();
		if (m_model->isDir(index)) {
			msg = tr("Delete Directory %1?").arg(m_model->fileName(index));
		} else {
			msg = tr("Delete File %1?").arg(m_model->fileName(index));
		}
	} else {
		msg = tr("Delete %1 Files?").arg(count);
	}

	if (QMessageBox::No == QMessageBox::warning(m_parent, "Delete",
			msg, QMessageBox::Yes, QMessageBox::No)) {
		return;
	}

//QElapsedTimer timer;
//timer.start();
	// Block all selectionModel signals to avoid emission on every removed file.
	bool oldState = m_selectionModel->blockSignals(true);

    QProgressDialog progress(m_parent);
    progress.setRange(0, count);
    progress.setModal(true);

	for (int i = 0; i < count; i++) {

		progress.setLabelText(tr("Delete %1").arg(m_model->filePath(m_selection.at(i))));
	        progress.setValue(i);
	        qApp->processEvents();
	        if (progress.wasCanceled()) {
			m_selectionModel->blockSignals(oldState);
			return;
		}

		if (!m_model->remove(m_selection.at(i))) {
			QMessageBox::critical(m_parent, QObject::tr("Error"),
				tr("Cannot Remove '%1'").arg(m_model->filePath(m_selection.at(i))),
				QMessageBox::Abort);
			m_selectionModel->blockSignals(oldState);
			return;
		}
	}
	m_selectionModel->blockSignals(oldState);

//qDebug() << "DELETE operation took" << timer.elapsed() << "milliseconds";
//	scrollToCurrent();
}

#if 0
bool Spreadsheet::writeFile(const QString &fileName)
{
    QFile file(fileName);
    ...
    QProgressDialog progress(this);
    progress.setLabelText(tr("Saving %1").arg(fileName));
    progress.setRange(0, RowCount);
    progress.setModal(true);

    for (int row = 0; row < RowCount; ++row) {
        progress.setValue(row);
        qApp->processEvents();
        if (progress.wasCanceled()) {
            file.remove();
            return false;
        }
        for (int column = 0; column < ColumnCount; ++column) {
            QString str = formula(row, column);
            if (!str.isEmpty())
                out << quint16(row) << quint16(column) << str;
        }
    }
    return true;
}
#endif

/*  SLOT [private] */
void FileManager::handleItemActivation(QModelIndex index)
{
	if (m_selectionModel->selectedIndexes().count() != 1) {
		// Dont bother on multiple file activation
		return;
	}

	if (m_model->isDir(index)) {
		// Change Path
	        m_currentDir.cd(m_model->filePath(index));
		m_currentIndex = m_model->setRootPath(m_currentDir.absolutePath());

		m_view->clearSelection();
		m_view->setRootIndex(m_currentIndex);
	        // Set view on firstRow
		m_view->setCurrentIndex(m_currentIndex.child(0, 0));
		emit currentPathChanged();
	}
}

void FileManager::previous()
{
	int row = m_view->currentIndex().row();
	QModelIndex mi = m_currentIndex.child(--row, 0);

	if (mi.isValid()) {
		m_view->setCurrentIndex(mi);
	}
}

void FileManager::next()
{
	int row = m_view->currentIndex().row();

	QModelIndex mi = m_currentIndex.child(++row, 0);
	if (mi.isValid()) {
		m_view->setCurrentIndex(mi);
	}
}

//
// Switches view mode [SLOT public]
// @param enable    (true = iconview, false = listview)
//
void FileManager::iconMode(bool enable)
{
	if (enable) {
		m_view->setViewMode(QListView::IconMode);
                m_view->setWordWrap(true);
		m_view->setGridSize(QSize(80,80));
	} else {
		m_view->setViewMode(QListView::ListMode);
		m_view->setWordWrap(false);
		m_view->setGridSize(QSize());
	}
}

// Switch view hidden files [SLOT public]
// @param enable    (true = show hidden files, false = listview)
void FileManager::showHidden(bool enable)
{
        if (enable) {
                m_model->setFilter(m_model->filter() | QDir::Hidden);
        } else {
                m_model->setFilter(m_model->filter() &~ QDir::Hidden);
        }
}

// [SLOT public]
void FileManager::mkdir()
{
	QString dirName = QInputDialog::getText(m_parent,
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
		QMessageBox::critical(m_parent,
			QObject::tr("Error"),
			QObject::tr("Creating dir '%1' failed").arg(dirName),
			QMessageBox::Abort);
	}
}

#if 0 // To be tested
void FileManager::scrollToCurrent()
{
	m_view->setUpdatesEnabled(false);
	// Used to select/scroll view
	int row = m_view->currentIndex().row();
	QModelIndex mi = m_currentIndex.child(row + 1, 0);

	if (mi.isValid()) {
		m_view->setCurrentIndex(mi);
	} else {
		mi = m_currentIndex.child(row - 1, 0);
		if (mi.isValid()) {
			m_view->setCurrentIndex(mi);
		}
	}
	m_view->setUpdatesEnabled(true);

//	next();

	qDebug() << "Current Index Row:" << m_view->currentIndex().row()
	<< m_model->fileName(m_view->currentIndex());

	m_view->scrollTo(m_view->currentIndex(), QAbstractItemView::PositionAtTop);
}
#endif


// [SLOT public]
void FileManager::renameSelectedFiles()
{
	QModelIndexList m_selection = m_selectionModel->selectedIndexes();

	if (m_selection.isEmpty()) {
		QMessageBox::critical(m_parent, tr("Error"), tr("No file(s) selected"), QMessageBox::Ok);
		return;
	}

	m_model->setReadOnly(false);

	if (m_selection.count() == 1) {
		// Single file
		RenameDialog dlg(m_model->fileName(m_selection.first()));
		if (dlg.exec() == QDialog::Accepted) {
			if (m_model->setData(m_selection.first(), dlg.newFileName())) {
				m_view->update(m_selection.first());
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
					m_view->update(m_selection.at(i));
				}
			}
		}
	}

	m_model->setReadOnly(true);
}

void FileManager::copy(const QString &destPath)
{
	m_selection = m_selectionModel->selectedIndexes();
	if (m_selection.isEmpty()) {
		QMessageBox::critical(m_parent, tr("Error"), tr("No file(s) selected"), QMessageBox::Ok);
		return;
	}

	if (destPath.isNull() || destPath.isEmpty()) {
		QMessageBox::critical(m_parent, tr("Error"), tr("Invalid destPath!"), QMessageBox::Ok);
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
			QMessageBox::critical(m_parent, tr("Copy Error"), tr("File '%1' already exists").arg(dest), QMessageBox::Abort);
			m_model->setReadOnly(true);
			return;
		}
		if (!QFile::copy(m_model->filePath(m_selection.first()), dest)) {
			QMessageBox::critical(m_parent, tr("Copy Error"),
				tr("Copying file '%1' failed").arg(m_model->fileName(m_selection.first())),
				QMessageBox::Abort);

			m_model->setReadOnly(true);
			return;
		}
		m_selection.removeFirst();
	}

	m_model->setReadOnly(true);
	m_view->clearSelection();
}

void FileManager::move(const QString &destPath)
{
	m_selection = m_selectionModel->selectedIndexes();
	if (m_selection.isEmpty()) {
		QMessageBox::critical(m_parent, tr("Error"), tr("No file(s) selected"), QMessageBox::Ok);
		return;
	}

	if (destPath.isNull() || destPath.isEmpty()) {
		QMessageBox::critical(m_parent, tr("Error"), tr("Invalid destPath!"), QMessageBox::Ok);
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
			QMessageBox::critical(m_parent, tr("Move Error"), tr("File '%1' already exists").arg(dest), QMessageBox::Abort);
			m_model->setReadOnly(true);
			return;
		}
		if (!QFile::rename(m_model->filePath(m_selection.first()), dest)) {
			QMessageBox::critical(m_parent, tr("Move Error"),
				tr("Moving file '%1' failed").arg(m_model->fileName(m_selection.first())),
				QMessageBox::Abort);

			m_model->setReadOnly(true);
			return;
		}
		m_selection.removeFirst();
	}
	m_model->setReadOnly(true);
	m_view->clearSelection();
}

