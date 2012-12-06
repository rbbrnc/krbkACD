#ifndef QEXIV2_H
#define QEXIV2_H

#include <QSharedDataPointer>
#include <QStringList>
#include <QString>
#include <QImage>
#include <QDateTime>

// For Tags
//#include "PTag.h"

#include "XmpRegion.h"

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

		bool isValid() const;

		bool load(const QString& filePath);
		bool loadFromData(const QByteArray& data);

		bool save();

		// EXIF Functions
		bool isExifWritable() const;
		bool hasExif() const;
		bool clearExif();
		QString exifTagString(const char *exifTagName, bool escapeCR) const;
		QDateTime exifTagDateTime(const char *exifTagName) const;

		// IPTC Functions
		bool isIptcWritable() const;
		bool hasIptc() const;
		bool clearIptc();

		// XMP Functions
		bool isXmpWritable() const;
		bool hasXmp() const;
		bool hasXmpRegionTag() const;

		QList<XmpRegion> xmpRegionList() const;

		bool removeXmpTag(const char *xmpTagName);
		QByteArray xmpPacket() const;

		QString xmpTagString(const char *xmpTagName, bool escapeCR) const;
		bool setXmpTagString(const char *xmpTagName, const QString &value);

		QStringList xmpTagStringBag(const char* xmpTagName, bool escapeCR) const;
		bool setXmpTagStringBag(const char *xmpTagName, const QStringList &bag);

		QStringList xmpTagStringSeq(const char *xmpTagName, bool escapeCR) const;
		bool setXmpTagStringSeq(const char *xmpTagName, const QStringList &seq);

		QString xmpTagStringLangAlt(const char *xmpTagName, const QString &langAlt, bool escapeCR);
		bool setXmpTagStringLangAlt(const char *xmpTagName, const QString &value, const QString &langAlt);

		// Image Comment Functions
		bool isImgCommentWritable() const;
		bool hasComment() const;
		bool clearImgComment();
		QByteArray imgComment() const;
		bool setImgComment(const QByteArray& data);

		QImage previewImage() const;

		QList<struct exifData> exifDataList() const;
		QList<struct exifData> iptcDataList() const;
		QList<struct exifData> xmpDataList() const;

	private:
		QList<XmpRegion> xmpMWGRegionList() const;
		QList<XmpRegion> xmpMPRegionList() const;

	private:
		QSharedDataPointer<QExiv2DataPrivate> d;
};

#endif
