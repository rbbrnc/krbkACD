#ifndef QEXIV2_H
#define QEXIV2_H

#include <QSharedDataPointer>
#include <QStringList>
#include <QString>
#include <QFileInfo>

// For Tags
#include "PTag.h"

// Forward decl.
class QExiv2DataPrivate;

class QExiv2
{
	public:
		QExiv2();
		QExiv2(const QExiv2 &other);
		QExiv2(const QString& filePath);
		~QExiv2();

		QExiv2& operator=(const QExiv2& other);

	public:
		bool load(const QString& filePath);
		bool loadFromData(const QByteArray& data);

		bool save();

		// EXIF Functions
		bool isExifWritable() const;
		bool hasExif() const;
		bool clearExif();

		// IPTC Functions
		bool isIptcWritable() const;
		bool hasIptc() const;
		bool clearIptc();

		// XMP Functions
		QString xmpTagString(const char *xmpTagName, bool escapeCR) const;
		QStringList xmpTagStringBag(const char* xmpTagName, bool escapeCR) const;

		// Image Comment Functions
		bool isImgCommentWritable() const;
		bool hasComment() const;
		bool clearImgComment();
		QByteArray imgComment() const;
		bool setImgComment(const QByteArray& data);

	public:
		// XXX PRELIMINARY!!!
		QList<PTag> xmpPTags() const;

	private:
		QSharedDataPointer<QExiv2DataPrivate> d;
};

#endif
