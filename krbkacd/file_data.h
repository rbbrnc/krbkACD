#ifndef FILE_DATA_H
#define FILE_DATA_H

#include <QPixmap>
#include "QExiv2.h"

class FileData {
	public:
		FileData();
		FileData(const QString &file);
		FileData(const QFileInfo &fileInfo);
		FileData(const FileData &other);
		~FileData();

		void print();

		bool isImage() const;
		QPixmap previewPixmap(int w, int h);
		QPixmap fullPixmap();

		const QExiv2 metadata() const;

		QFileInfo  fileInfo() const;
		QString    filePath() const;
		QString    mimeType() const;
		QByteArray md5();
		QByteArray imageMd5();

		QImage image();

	private:
		void setMimeType();
		void setMd5();

	private:
		QFileInfo  m_fileInfo;
		QString    m_path;
		QString    m_mimeType;
		QByteArray m_md5;
		QPixmap    m_pixmap;
		QImage	   m_image;
		QExiv2     m_metadata;
};

#endif
