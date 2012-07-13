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

		QString mimeType();

	private:
		void setMimeType();
		void setMd5();
		QByteArray md5();

	private:
		QString m_path;
		QString m_mimeType;
		QByteArray m_md5;

		QFileInfo m_fileInfo;
		QPixmap m_pixmap;

		QExiv2 m_metadata;
};

#endif
