#ifndef QEXIV2_H
#define QEXIV2_H

#include <QSharedDataPointer>
#include <QStringList>
#include <QString>
#include <QImage>
#include <QDateTime>
#include <QGeoCoordinate>

#include "mwg_region.h"
#include "md_location.h"
#include "md_social.h"

//#define ENABLE_XMP_SERIALIZE 1
//#define ENABLE_EXIF_PREVIEW  1

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
		QExiv2(const QString& filePath);
		~QExiv2();

		enum DateTimeType {
			ExifOriginal = 0,
			ExifDigitized,
			ExifModified,
			IptcOriginal,
			IptcDigitized,
			XmpOriginal,
			XmpDigitized,
			XmpModified
		};

        QString errorString() const;

		// General metadata methods
		bool isValid() const;
		bool load(const QString& filePath);
        bool loadFromData(const QByteArray& data);

		bool save();

		bool hasExif() const;
		bool hasIptc() const;
		bool hasXmp() const;

		// EXIF Functions
		QString exifTagString(const char *tag, bool escapeCR = false) const;
        QByteArray exifTagData(const char *tag) const;

        bool isExifWritable() const;
        void clearExif();

        // Date-Time Functions
		bool setDatetime(const QDateTime &dt, QExiv2::DateTimeType type);
		QDateTime datetime(QExiv2::DateTimeType type) const;

#ifdef ENABLE_EXIF_PREVIEW
		// Exif preview (thumbnail) image
		QImage previewImage() const;
#endif

		// IPTC Functions
        bool isIptcWritable() const;
        void clearIptc();

        QString iptcTagString(const char *tag, bool escapeCR = false) const;

		// XMP Functions
		bool isXmpWritable() const;
        void clearXmp();

        bool xmpRegisterNamespace(const QString& uri, const QString &prefix);

		QString xmpTagString(const char *tag, bool escapeCR = false) const;
		QString xmpTagString(const QString &tag, bool escapeCR = false) const;
		bool setXmpTagString(const char *tag, const QString &value);
		bool setXmpTagString(const QString &tag, const QString& value);

		QStringList xmpTagStringBag(const char *tag, bool escapeCR = false) const;
        bool setXmpBag(const char *tag, const QStringList &bag);
		bool setXmpTagBag(const char *tag);


		QStringList xmpTagStringSeq(const char *tag, bool escapeCR = false) const;
		bool setXmpTagStringSeq(const char *tag, const QStringList &seq);

        QString xmpLangAlt(const char *tag, const QString &langAlt, bool escapeCR) const;
        bool setXmpLangAlt(const char *tag, const QString &value, const QString &langAlt = QStringLiteral("x-default"));

		bool removeXmpTag(const char *tag);
		void removeXmpBag(const char *tag, int tagNameSize);

#ifdef ENABLE_XMP_SERIALIZE
		QByteArray xmpPacket() const;
#endif

		// XMP Regions
		bool xmpHasRegionTags() const;
		void xmpEraseRegions();
		MwgRegionList xmpMwgRegionList() const;
		void xmpSetMwgRegionList(const MwgRegionList &regions);
		void xmpSetMwgRegion(const MwgRegion &region, int n);

		// Image Comment Functions
		bool isImgCommentWritable() const;
		bool hasComment() const;
        void clearImgComment();
		QByteArray imgComment() const;
        void setImgComment(const QByteArray &data);

		QList<struct exifData> exifDataList() const;
		QList<struct exifData> iptcDataList() const;
		QList<struct exifData> xmpDataList() const;

		// Social
		SocialMetadata socialMetadata();
		bool setSocialMetadata(const SocialMetadata &smd, bool writeComments);

		// Locations
        void locationShown(MetadataLocation &loc, int index = 1);
        void setLocationShown(MetadataLocation &loc, int index = 1);
		void locationCreated(MetadataLocation &loc, int index = 1);
		void setLocationCreated(MetadataLocation &loc, int index = 1);

        // GPS
        QDateTime gpsTimestamp() const;

		QGeoCoordinate geoCoordinate() const;
		QGeoCoordinate exifGeoCoordinate() const;
		QGeoCoordinate xmpGeoCoordinate() const;

	private:

        QStringList xmpStringList(const char *tag, bool escapeCR) const;
        bool setXmpStringList(const char *tag, const QStringList &list, int typeId);

		void setLocation(MetadataLocation &loc, const QString &locType, int index);
		void location(MetadataLocation &loc, const QString &locType, int index);

        double gpsCoordinates(const double deg, const double min, const double sec, const QChar &dir) const;

        bool xmpGpsCoordinates(const QString &str, double *value) const;
        bool exifGpsCoordinate(const QString &coordTag, const QString &refTag, double *value) const;

        //bool xmpGpsAltitude(double *altitude) const;
        //bool exifGpsAltitude(double *altitude) const;

        void setErrorString(const QString &msg);
        bool setXmpTag(const char *tag, const QString &value, int typeId);

    private:
		QSharedDataPointer<QExiv2DataPrivate> d;

        QString m_errorString;
};

#endif
