#ifndef QEXIV2_H
#define QEXIV2_H

#include <QSharedDataPointer>
#include <QStringList>
#include <QString>
#include <QImage>
#include <QDateTime>

//#include "XmpRegion.h"
#include "mwg_region.h"
#include "mwg_region_list.h"

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

		// General metadata methods
		bool isValid() const;
		bool load(const QString& filePath);
		bool loadFromData(const QByteArray& data);
		bool save();

		bool hasExif() const;
		bool hasIptc() const;
		bool hasXmp() const;

		// EXIF Functions
		bool isExifWritable() const;
		bool clearExif();
		QString exifTagString(const char *tag, bool escapeCR = false) const;
		QDateTime exifTagDateTime(const char *tag) const;

		// Exif preview (thumbnail) image
		QImage previewImage() const;

		// IPTC Functions
		bool isIptcWritable() const;
		bool clearIptc();
		QString iptcTagString(const char *tag, bool escapeCR = false) const;

		// XMP Functions
		bool isXmpWritable() const;

		QString xmpTagString(const char *tag, bool escapeCR = false) const;
		QString xmpTagString(const QString &tag, bool escapeCR = false) const;
		bool setXmpTagString(const char *tag, const QString &value);
		bool setXmpTagString(const QString &tag, const QString& value);

		QStringList xmpTagStringBag(const char *tag, bool escapeCR = false) const;
		bool setXmpTagStringBag(const char *tag, const QStringList &bag);
		bool setXmpTagBag(const char *tag);

		QStringList xmpTagStringSeq(const char *tag, bool escapeCR = false) const;
		bool setXmpTagStringSeq(const char *tag, const QStringList &seq);

		QString xmpTagStringLangAlt(const char *tag, const QString &langAlt, bool escapeCR);
		bool setXmpTagStringLangAlt(const char *tag, const QString &value, const QString &langAlt);

		bool removeXmpTag(const char *tag);
		void removeXmpBag(const char *tag, int tagNameSize);
		QByteArray xmpPacket() const;

		// XMP Regions
		bool xmpHasRegionTags() const;
		void xmpEraseRegions();
		MwgRegionList xmpMwgRegionList() const;
		void xmpSetMwgRegionList(const MwgRegionList &regions);
		void xmpSetMwgRegion(const MwgRegion &region, int n);

		// Image Comment Functions
		bool isImgCommentWritable() const;
		bool hasComment() const;
		bool clearImgComment();
		QByteArray imgComment() const;
		bool setImgComment(const QByteArray& data);

		QList<struct exifData> exifDataList() const;
		QList<struct exifData> iptcDataList() const;
		QList<struct exifData> xmpDataList() const;

	private:
		// Readed from file not metadata!!
		int m_imageWidth;
		int m_imageHeight;

		QSharedDataPointer<QExiv2DataPrivate> d;
};

#endif
