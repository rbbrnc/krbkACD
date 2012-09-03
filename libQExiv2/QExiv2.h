#ifndef QEXIV2_H
#define QEXIV2_H

#include <QSharedDataPointer>
#include <QStringList>
#include <QString>
#include <QImage>
#include <QDateTime>

// For Tags
#include "PTag.h"

// Forward decl.
class QExiv2DataPrivate;


struct exifData {
	QString family;
	QString group;
	QString tagName;
	QString key;
	unsigned int tag;
	QString typeName;
	unsigned int typeId;
	long count;
	QString value;
};

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
		QString getExifTagString(const char *exifTagName, bool escapeCR) const;
		QDateTime getExifTagDateTime(const char *exifTagName) const;

		// IPTC Functions
		bool isIptcWritable() const;
		bool hasIptc() const;
		bool clearIptc();

		// XMP Functions
		bool isXmpWritable() const;
		bool hasXmp() const;

		bool removeXmpTag(const char *xmpTagName);
		QByteArray xmpPacket() const;

		QString xmpTagString(const char *xmpTagName, bool escapeCR) const;
		bool setXmpTagString(const char *xmpTagName, const QString &value);

		QStringList xmpTagStringBag(const char* xmpTagName, bool escapeCR) const;
		bool setXmpTagStringBag(const char *xmpTagName, const QStringList &bag);

		QString getXmpTagStringLangAlt(const char *xmpTagName, const QString &langAlt, bool escapeCR);
		bool setXmpTagStringLangAlt(const char *xmpTagName, const QString &value, const QString &langAlt);
		QString detectLanguageAlt(const QString& value, QString& lang);

		// Image Comment Functions
		bool isImgCommentWritable() const;
		bool hasComment() const;
		bool clearImgComment();
		QByteArray imgComment() const;
		bool setImgComment(const QByteArray& data);


		QImage getPreviewImage() const;

	public:
		// XXX PRELIMINARY!!!
		QList<PTag> xmpPTags() const;
		QList<PTag> xmpMWG_RegionsTags() const;
		QList<struct exifData> exifDataList() const;
		QList<struct exifData> iptcDataList() const;
		QList<struct exifData> xmpDataList() const;

	private:
		QSharedDataPointer<QExiv2DataPrivate> d;
};

#endif
