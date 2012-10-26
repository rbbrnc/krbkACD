#include <QDir>
#include <QDebug>

#include "file_utils.h"
#include "file_data.h"
#include "QMagic.h"

FileData::FileData() :
	m_isImage(false)
{
}

FileData::FileData(const QFileInfo &fileInfo) :
	m_fileInfo(fileInfo)
{
	init();
}

FileData::FileData(const QString &file) :
	m_fileInfo(QFileInfo(file))
{
	init();
}

FileData::FileData(const FileData &other) :
	m_fileInfo(other.m_fileInfo),
	m_path(other.m_path),
	m_mimeType(other.m_mimeType),
	m_md5(other.m_md5),
	m_pixmap(other.m_pixmap),
	m_image(other.m_image),
	m_metadata(other.m_metadata),
	m_isImage(other.m_isImage)
{
}

void FileData::init()
{
	m_path     = m_fileInfo.absoluteFilePath();
	m_mimeType = QMagic::mimeType(m_path);
	m_isImage  = m_mimeType.contains("image/", Qt::CaseInsensitive);

	// load metadata only for mime type "image/xxxx"
	if (isImage()) {
		//qDebug() << __PRETTY_FUNCTION__ << "- load" << m_path;
		m_image.load(m_path);
		if (!m_image.isNull()) {
			m_metadata.load(m_path);
		}
	}
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
	return m_isImage;
}

QPixmap FileData::previewPixmap(int w, int h)
{
	QImage preview = m_metadata.previewImage();
	if (preview.isNull()) {
		if (m_image.isNull()) {
			return QPixmap();
		} else {
			preview = m_image;
		}
	}
	//qDebug() << __PRETTY_FUNCTION__ - use EXIF preview";
	QPixmap p;
	p.convertFromImage(preview);
	return p.scaled(w, h, Qt::KeepAspectRatio);
}

QImage FileData::image()
{
	return m_image;
}

QPixmap FileData::fullPixmap()
{
	if (m_image.isNull()) {
		return QPixmap();
	}

	QPixmap p;
	p.convertFromImage(m_image);
	return p;
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
	if (m_image.isNull()) {
		return QByteArray();
	}
	QByteArray ba((const char *) m_image.constBits());
	return ::md5(ba);
}
