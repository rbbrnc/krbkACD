#include <QDir>

#include <QDebug>

#include "file_utils.h"
#include "file_data.h"


FileData::FileData()
{
}

FileData::FileData(const QFileInfo &fileInfo) :
	m_fileInfo(fileInfo)
{
	m_path = fileInfo.absoluteFilePath();

	m_mimeType = ::mimeType(m_path);

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

	m_mimeType = ::mimeType(m_path);

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
	QImage preview = m_metadata.previewImage();
	if (preview.isNull()) {
		if (m_pixmap.load(m_path)) {
			return m_pixmap.scaled(w, h, Qt::KeepAspectRatio);
		}

		return QPixmap();
	}
	//qDebug() << __PRETTY_FUNCTION__ - use EXIF preview";
	QPixmap p;
	p.convertFromImage(preview);
	return p;
}

QImage FileData::image()
{
	if (m_image.isNull()) {
		//qDebug() << __PRETTY_FUNCTION__ - load" << m_path;
		m_image.load(m_path);
	}

	return m_image;
}

QPixmap FileData::fullPixmap()
{
#if 10
	if (m_image.isNull()) {
		//qDebug() << __PRETTY_FUNCTION__ - load" << m_path;
		m_image.load(m_path);
	}

	QPixmap p;
	p.convertFromImage(m_image);
	return p;
#else
	if (m_pixmap.load(m_path)) {
		//qDebug() << __PRETTY_FUNCTION__ - load" << m_path;
		return m_pixmap;
	}

	return QPixmap();
#endif
}

const QExiv2 FileData::const_metadata() const
{
	return m_metadata;
}

QExiv2 *FileData::metadata()
{
	return &m_metadata;
}

QByteArray FileData::md5()
{
	if (m_md5.isEmpty()) {
		m_md5 = ::md5(m_path);
	}
	return m_md5;
}

QByteArray FileData::imageMd5()
{
	QImage img = image();
	if (img.isNull()) {
		return QByteArray();
	}

	QByteArray ba((const char *) img.constBits());
	return ::md5(ba);
}

void FileData::print()
{
	md5();
	qDebug() << __PRETTY_FUNCTION__
		 << "FILE:" << qPrintable(m_path)
		 << m_mimeType
		 << m_md5.toHex();
}
