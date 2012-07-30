#include <magic.h>

#include <QDir>
#include <QCryptographicHash>

#include <QDebug>

#include "file_data.h"


FileData::FileData()
{
}

FileData::FileData(const QFileInfo &fileInfo) :
	m_fileInfo(fileInfo)
{
	m_path = fileInfo.absoluteFilePath();

	setMimeType();

	// load metadata only for mime type "image/xxxx"
	if (isImage()) {
		m_image = QImage();
		m_metadata.load(m_path);
	}
}

FileData::FileData(const QString &file)
{
	m_fileInfo = QFileInfo(file);
	m_path = m_fileInfo.absoluteFilePath();

	setMimeType();

	// load metadata only for mime type "image/xxxx"
	if (isImage()) {
		m_image = QImage();
		m_metadata.load(m_path);
	}
}

FileData::FileData(const FileData &other) :
	m_fileInfo(other.m_fileInfo),
	m_path(other.m_path),
	m_mimeType(other.m_mimeType),
	m_md5(other.m_md5),
	m_pixmap(other.m_pixmap),
	m_image(other.m_image),
	m_metadata(other.m_metadata)
{
}

FileData::~FileData()
{
}

QString FileData::filePath() const
{
	return m_path;
}

QFileInfo FileData::fileInfo() const
{
	return m_fileInfo;
}

QString FileData::mimeType() const
{
	return m_mimeType;
}

bool FileData::isImage() const
{
	return m_mimeType.contains("image/", Qt::CaseInsensitive);
}

QPixmap FileData::previewPixmap(int w, int h)
{
	QImage preview = m_metadata.getPreviewImage();
	if (preview.isNull()) {
		if (m_pixmap.load(m_path)) {
			return m_pixmap.scaled(w, h, Qt::KeepAspectRatio);
		}

		return QPixmap();
	}
//	qDebug() << "FileData::previewPixmap() - use EXIF preview";
	QPixmap p;
	p.convertFromImage(preview);
	return p;
}

QImage FileData::image()
{
	if (m_image.isNull()) {
//		qDebug() << "FileData::image() - load" << m_path;
		m_image.load(m_path);
	}

	return m_image;
}

QPixmap FileData::fullPixmap()
{
#if 10
	if (m_image.isNull()) {
//		qDebug() << "FileData::fullPixmap() - load" << m_path;
		m_image.load(m_path);
	}

	QPixmap p;
	p.convertFromImage(m_image);
	return p;
#else
	if (m_pixmap.load(m_path)) {
		qDebug() << "FileData::fullPixmap() - load" << m_path;
		return m_pixmap;
	}

	return QPixmap();
#endif
}

const QExiv2 FileData::metadata() const
{
	return m_metadata;
}

void FileData::setMimeType()
{
	magic_t cookie;

	/* cfr. man libmagic */
	cookie = magic_open(MAGIC_MIME | MAGIC_NO_CHECK_ASCII | MAGIC_NO_CHECK_ELF);
	if (cookie == NULL) {
		qDebug() << __func__ << "Error allocating magic cookie";
	} else {
		// load magic file (NULL default)
		// XXX: Can be a custom magic file on .qrc?!
		if (magic_load(cookie, NULL /*const char *filename*/) != 0) {
			qDebug() << __func__ << "Error loading magic data";
		} else {
			const char *s = magic_file(cookie, m_path.toAscii().constData());
			if (s == NULL) {
				qDebug() << "FILE:" << qPrintable(m_path) << magic_error(cookie);
			} else {
				m_mimeType = QString(s);
			}
		}
		magic_close(cookie);
	}
}

void FileData::setMd5()
{
	QFile file(m_path);

	if (!file.open(QIODevice::ReadOnly))
		return;

	QByteArray byteArray = file.readAll();
	m_md5 = QCryptographicHash::hash(byteArray, QCryptographicHash::Md5);
}

QByteArray FileData::md5()
{
	if (m_md5.isEmpty()) {
		setMd5();
	}
	return m_md5;
}

QByteArray FileData::imageMd5()
{
	QByteArray md5;
	QImage img = image();
	if (!img.isNull()) {
		QByteArray ba((const char *) img.constBits());
		md5 = QCryptographicHash::hash(ba, QCryptographicHash::Md5);
	}
	return md5;
}

void FileData::print()
{
	md5();
	qDebug() << "FILE:" << qPrintable(m_path) << m_mimeType << m_md5.toHex();
}

