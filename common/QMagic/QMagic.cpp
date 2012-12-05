// For libmagic
#include <magic.h>

#include <QDebug>
#include <QImage>

#include "QMagic.h"

static QString getMagic(const QString &file, int flags)
{
	magic_t cookie;
	QString magicString;


	if ((file.isNull()) || (file.isEmpty())) {
		qDebug() << __PRETTY_FUNCTION__ << "NULL";
		return magicString;
	}

	/* cfr. man libmagic */
	cookie = magic_open(flags);
	if (cookie == NULL) {
		qDebug() << __PRETTY_FUNCTION__ << "Error allocating magic cookie";
	} else {
		// load magic file (NULL default)
		// XXX: Can be a custom magic file on .qrc?!
		if (magic_load(cookie, NULL /*const char *filename*/) != 0) {
			qDebug() << __PRETTY_FUNCTION__ << "Error loading magic data";
		} else {
			const char *s = magic_file(cookie, file.toAscii().constData());
			if (s == NULL) {
				qDebug() << __PRETTY_FUNCTION__
					 << "FILE:" << qPrintable(file)
					 << magic_error(cookie);
			}
			magicString = QString(s);
		}
		magic_close(cookie);
	}

	return magicString;
}

// Get MIME description with libmagic
QString QMagic::mimeDescription(const QString &file)
{
	return getMagic(file, MAGIC_NO_CHECK_ASCII | MAGIC_NO_CHECK_ELF);
}

// Get MIME type with libmagic
QString QMagic::mimeType(const QString &file)
{
	return getMagic(file, MAGIC_MIME | MAGIC_NO_CHECK_ASCII | MAGIC_NO_CHECK_ELF);
}

bool QMagic::mimeTypeIsImage(const QString &file)
{
	QString mime = getMagic(file, MAGIC_MIME | MAGIC_NO_CHECK_ASCII | MAGIC_NO_CHECK_ELF);
	return mime.contains("image");
}

// Fill the QMimeData class
void QMagic::mimeData(QMimeData &mimeData, const QString &file)
{
	QByteArray data;
	QString mime = QMagic::mimeType(file);

	mimeData.setData(mime, data);

	if (mime.contains("image")) {
		mimeData.setImageData(QImage(file));
	}
}
