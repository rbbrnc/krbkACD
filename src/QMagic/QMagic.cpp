#include "qglobal.h"
#if QT_VERSION >= 0x050000
#include <QMimeDatabase>
#else
#include <magic.h>	// For libmagic
#define USE_LIBMAGIC
#endif

#include <QDebug>
#include <QImage>

#include "QMagic.h"

#ifdef USE_LIBMAGIC
static QString getMagic(const QString &file, int flags)
{
	magic_t cookie;
	QString magicString;

	if ((file.isNull()) || (file.isEmpty())) {
        qDebug() << Q_FUNC_INFO << "NULL";
		return magicString;
	}

	/* cfr. man libmagic */
	cookie = magic_open(flags);
	if (cookie == NULL) {
        qDebug() << Q_FUNC_INFO << "Error allocating magic cookie";
	} else {
		// load magic file (NULL default)
		// XXX: Can be a custom magic file on .qrc?!
		if (magic_load(cookie, NULL /*const char *filename*/) != 0) {
            qDebug() << Q_FUNC_INFO << "Error loading magic data";
		} else {
			const char *s = magic_file(cookie, file.toLatin1().constData());
			if (s == NULL) {
                qDebug() << Q_FUNC_INFO
					 << "FILE:" << qPrintable(file)
					 << magic_error(cookie);
			}
			magicString = QString(s);
		}
		magic_close(cookie);
	}

	return magicString;
}
#endif

namespace QMagic {

QString mimeType(const QString &file)
{
#ifdef USE_LIBMAGIC
	// Get MIME type with libmagic
	return getMagic(file, MAGIC_MIME | MAGIC_NO_CHECK_ASCII | MAGIC_NO_CHECK_ELF);
#else
	// Note: Due to a Qt 5.0.1 bug, mimeTypeForFile(QString, MatchMode) doesn't
	// function properly for mode == MatchContent.
	QMimeDatabase mimeDatabase;
	return mimeDatabase.mimeTypeForFile(file, QMimeDatabase::MatchContent).name();
#endif
}

bool mimeTypeIsImage(const QString &file)
{
#ifdef USE_LIBMAGIC
	QString mime = getMagic(file, MAGIC_MIME | MAGIC_NO_CHECK_ASCII | MAGIC_NO_CHECK_ELF);
	return mime.contains("image");
#else
	// Note: Due to a Qt 5.0.1 bug, mimeTypeForFile(QString, MatchMode) doesn't
	// function properly for mode == MatchContent.
	QMimeDatabase mimeDatabase;
	return mimeDatabase.mimeTypeForFile(file,
			QMimeDatabase::MatchContent).name().contains("image");
#endif
}

} // end-namespace "QMagic"
