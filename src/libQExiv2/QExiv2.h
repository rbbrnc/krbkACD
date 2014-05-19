#ifndef QEXIV2_H
#define QEXIV2_H

#include <QSharedDataPointer>
#include <QStringList>
#include <QString>
#include <QImage>
#include <QDateTime>

#include "mwg_region.h"

//#define ENABLE_XMP_SEQ       1
//#define ENABLE_XMP_SERIALIZE 1
//#define ENABLE_EXIF_WRITE    1
//#define ENABLE_EXIF_PREVIEW  1
//#define ENABLE_IPTC_WRITE    1

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

		// Unused
		//bool loadFromData(const QByteArray& data);

		bool save();

		bool hasExif() const;
		bool hasIptc() const;
		bool hasXmp() const;

		// EXIF Functions
		QString exifTagString(const char *tag, bool escapeCR = false) const;

#ifdef ENABLE_EXIF_WRITE
		bool isExifWritable() const;
		bool clearExif();
#endif
        // Date-Time Functions
        QDateTime exifDateTimeOriginal() const;
        QDateTime exifDateTimeDigitized() const;
        QDateTime exifDateTime() const;

#ifdef ENABLE_EXIF_PREVIEW
		// Exif preview (thumbnail) image
		QImage previewImage() const;
#endif

		// IPTC Functions
#ifdef ENABLE_IPTC_WRITE
		bool isIptcWritable() const;
		bool clearIptc();
#endif
		QString iptcTagString(const char *tag, bool escapeCR = false) const;

        // Date-Time Functions
        QDateTime iptcDateTimeCreated() const;
        QDateTime iptcDateTimeDigitized() const;

		// XMP Functions
		bool isXmpWritable() const;
		bool xmpRegisterNamespace(const QString& uri, const QString& prefix);

		QString xmpTagString(const char *tag, bool escapeCR = false) const;
		QString xmpTagString(const QString &tag, bool escapeCR = false) const;
		bool setXmpTagString(const char *tag, const QString &value);
		bool setXmpTagString(const QString &tag, const QString& value);

		QStringList xmpTagStringBag(const char *tag, bool escapeCR = false) const;
		bool setXmpTagStringBag(const char *tag, const QStringList &bag);
		bool setXmpTagBag(const char *tag);

#ifdef ENABLE_XMP_SEQ
		QStringList xmpTagStringSeq(const char *tag, bool escapeCR = false) const;
		bool setXmpTagStringSeq(const char *tag, const QStringList &seq);
#endif

		QString xmpTagStringLangAlt(const char *tag, const QString &langAlt, bool escapeCR);
		bool setXmpTagStringLangAlt(const char *tag, const QString &value, const QString &langAlt);

		bool removeXmpTag(const char *tag);
		void removeXmpBag(const char *tag, int tagNameSize);

#ifdef ENABLE_XMP_SERIALIZE
		QByteArray xmpPacket() const;
#endif
        // XMP Date-Time
        QDateTime xmpDateTimeCreated() const;
        QDateTime xmpDateTimeDigitized() const;
        QDateTime xmpDateTimeModified() const;


		// XMP Regions
		bool xmpHasRegionTags() const;
		void xmpEraseRegions();
		MwgRegionList xmpMwgRegionList() const;
		void xmpSetMwgRegionList(const MwgRegionList &regions);
		void xmpSetMwgRegion(const MwgRegion &region, int n);

		// Image Comment Functions
		bool isImgCommentWritable() const;
		bool hasComment() const;
		//bool clearImgComment();
		QByteArray imgComment() const;
		bool setImgComment(const QByteArray& data);

		QList<struct exifData> exifDataList() const;
		QList<struct exifData> iptcDataList() const;
		QList<struct exifData> xmpDataList() const;

	private:
		QSharedDataPointer<QExiv2DataPrivate> d;
};

#endif
