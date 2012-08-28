#include <QMessageBox>
#include <QInputDialog>
#include <QDir>
#include <QFileInfo>

#include <QDebug>

#include "file_utils.h"

/* TODO
 *
 * Add Warning on delete directory not empty!
 * Batch rename
 * Confirm on Copy & Move
 *
 */

/* Rename file */
void renameFile(QString filePath, QWidget *parent)
{
	QFileInfo fi(filePath);
	QString newName = QInputDialog::getText(parent,
				QObject::tr("Rename"),
	                        QObject::tr("New filename: "),
				QLineEdit::Normal,
				fi.fileName());

	if ((newName != fi.fileName()) && (!newName.isEmpty())) {
		if (!QFile::rename(fi.absoluteFilePath(), fi.absolutePath() + "/" + newName)) {
			QMessageBox::critical(parent,
			      QObject::tr("Error"),
			      QObject::tr("Renaming file '%1' failed").arg(fi.fileName()),
			      QMessageBox::Abort);
		}
	}
}

/* Delete File */
void deleteFile(QString filePath, QWidget *parent)
{
	if (QMessageBox::Yes == QMessageBox::warning(parent,
			QObject::tr("Delete"),
			QObject::tr("Delete '%1' ?").arg(filePath),
			QMessageBox::Yes,
			QMessageBox::No))
	{
		if (!QFile::remove(filePath)) {
			QMessageBox::critical(parent,
			      QObject::tr("Error"),
			      QObject::tr("Deleting file '%1' failed").arg(filePath),
			      QMessageBox::Abort);
		}
	}
}

bool copyFile(const QString destPath, const QString filePath, QWidget *parent)
{
	QFileInfo fin(filePath);
	QFileInfo fout(destPath + "/" + fin.fileName());

	if (fout.exists()) {
		QMessageBox::critical(parent,
			QObject::tr("Error"),
			QObject::tr("File '%1' already exists").arg(fout.absoluteFilePath()),
			QMessageBox::Abort);
		return false;
	}

	if (QFile::copy(fin.absoluteFilePath(), fout.absoluteFilePath())) {
		return true;
	}

	QMessageBox::critical(parent,
		QObject::tr("Error"),
		QObject::tr("Copying file '%1' failed").arg(fin.fileName()),
		QMessageBox::Abort);

	return false;
}

/*
 * Move files that were selected when called
 *
 * destPat  = Destination Path
 * filePath = file absolute path
 *
 */
bool moveFile(const QString destPath, const QString filePath, QWidget *parent)
{
	QFileInfo fi(filePath);

	if (QFile::rename(fi.absoluteFilePath(), destPath + "/" + fi.fileName())) {
		return true;
	}

	QMessageBox::critical(parent,
		QObject::tr("Error"),
		QObject::tr("Moving file '%1' failed").arg(fi.fileName()),
		QMessageBox::Abort);
	return false;
}

/* Create New Directory
 * destPath = base path
 */
void makeDir(const QString destPath, QWidget *parent)
{
	QString dirName = QInputDialog::getText(parent,
				QObject::tr("Create a new directory"),
				QObject::tr("Enter directory name: "),
				QLineEdit::Normal);
	// Remove whitespaces
	dirName = dirName.simplified();

	if (dirName.isEmpty()) {
		return;
	}

	QDir newDir;

	if (!newDir.mkdir(destPath + "/" + dirName)) {
		QMessageBox::critical(parent,
			QObject::tr("Error"),
			QObject::tr("Creating dir '%1' failed").arg(dirName),
			QMessageBox::Abort);
	}
}
