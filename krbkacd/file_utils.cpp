// For libmagic
#include <magic.h>

// For MD5 digest
#include <QCryptographicHash>

#include <QMessageBox>
#include <QInputDialog>
#include <QDir>
#include <QFileInfo>

#include <QDebug>

#include "RenameDialog.h"
#include "BatchRenameDialog.h"

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
	RenameDialog dlg(fi.absoluteFilePath());
	if (dlg.exec() != QDialog::Accepted) {
		return;
	}

	QString newName = dlg.newFileName();

	if (!QFile::rename(fi.absoluteFilePath(), newName)) {
		QMessageBox::critical(parent,
		      QObject::tr("Error"),
		      QObject::tr("Renaming file '%1' failed").arg(fi.fileName()),
		      QMessageBox::Abort);
	}
}

/* Rename files */
void renameFiles(QStringList files, QWidget *parent)
{
	if (files.isEmpty()) {
		qDebug() << __PRETTY_FUNCTION__ << "Empty file list!";
		return;
	}

	QStringList *filesOut = 0;
	int n = files.count();
	if (n == 1) {
		RenameDialog dlg(files.at(0));
		if (dlg.exec() == QDialog::Accepted) {
			filesOut = new QStringList(dlg.newFileName());
		} else {
			qDebug() << "Single Reject";
		}
	} else if (n > 1) {
		BatchRenameDialog dlg(files);
		if (dlg.exec() == QDialog::Accepted) {
			filesOut = new QStringList(dlg.newFileNames());
		} else {
			qDebug() << "Multi Reject";
		}
	} else {
		qDebug() << __PRETTY_FUNCTION__ << "Error 3";
		qDebug() << "Input:" << files;
		return;
	}

	if (filesOut) {
		for (int i = 0; i < filesOut->count(); i++) {
			qDebug() << "In:" << files.at(i) << "-->" << filesOut->at(i);
			if (!QFile::rename(files.at(i), filesOut->at(i))) {
				QMessageBox::critical(parent,
				      QObject::tr("Error"),
				      QObject::tr("Renaming file '%1' failed").arg(files.at(i)),
				      QMessageBox::Abort);
			}
		}
		delete filesOut;
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

/* Delete File(s) */
void deleteFiles(QStringList fileList, QWidget *parent)
{
	if (fileList.isEmpty()) {
		QMessageBox::critical(parent,
			QObject::tr("Delete File(s)"),
			QObject::tr("No file(s) selected"),
			QMessageBox::Abort);
		return;
	}

	int count = fileList.size();
	if (count == 1) {
		deleteFile(fileList.at(0), parent);
		return;
	}

	if (QMessageBox::Yes == QMessageBox::warning(parent,
				QObject::tr("Delete"),
				QObject::tr("Delete %1 files?").arg(count),
				QMessageBox::Yes,
				QMessageBox::No))
	{
		//delete files until filelist empty or error occured
		for (int i = 0; i < count; i++) {
			QFile file(fileList.at(i));
			if (!file.remove()) {
				QMessageBox::critical(parent,
				      QObject::tr("Error"),
				      QObject::tr("Deleting file '%1' failed").arg(file.fileName()),
				      QMessageBox::Ok);
				break;
			}
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

// Get MIME type with libmagic
QString mimeType(const QString &filePath)
{
	magic_t cookie;
	QString mimeType;

	/* cfr. man libmagic */
	cookie = magic_open(MAGIC_MIME | MAGIC_NO_CHECK_ASCII | MAGIC_NO_CHECK_ELF);
	if (cookie == NULL) {
		qDebug() << __PRETTY_FUNCTION__ << "Error allocating magic cookie";
	} else {
		// load magic file (NULL default)
		// XXX: Can be a custom magic file on .qrc?!
		if (magic_load(cookie, NULL /*const char *filename*/) != 0) {
			qDebug() << __PRETTY_FUNCTION__ << "Error loading magic data";
		} else {
			const char *s = magic_file(cookie, filePath.toAscii().constData());
			if (s == NULL) {
				qDebug() << __PRETTY_FUNCTION__
					 << "FILE:" << qPrintable(filePath)
					 << magic_error(cookie);
			}
			mimeType = QString(s);
		}
		magic_close(cookie);
	}

	return mimeType;
}

// MD5 digest of file
QByteArray md5(const QString &filePath)
{
	QFile file(filePath);

	if (!file.open(QIODevice::ReadOnly))
		return QByteArray();

	QByteArray byteArray = file.readAll();
	return QCryptographicHash::hash(byteArray, QCryptographicHash::Md5);
}

QByteArray md5(const QByteArray &data)
{
	return QCryptographicHash::hash(data, QCryptographicHash::Md5);
}

