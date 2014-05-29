#include <QFile>
#include <QFileInfo>
#include <QDebug>

#include "QExiv2.h"
#include "qexiv2_p.h"

// Date-Time Functions
//
// [Original]  Creation date of the intellectual content (e.g. when a photo was taken)
// [Digitized] Creation date of the digital representation (e.g. when an image was digitized)
// [Modified]  Modification date of the digital image file (e.g. when a file was modified by the user)

// Set a DateTime time Tag
bool QExiv2::setDatetime(const QDateTime &dt, QExiv2::DateTimeType type)
{
	switch (type) {
	case QExiv2::XmpOriginal:
		setXmpTagString("Xmp.photoshop.DateCreated", dt.toString(Qt::ISODate));
		break;
	case QExiv2::ExifOriginal:
	case QExiv2::ExifDigitized:
	case QExiv2::ExifModified:
	case QExiv2::IptcOriginal:
	case QExiv2::IptcDigitized:
	case QExiv2::XmpDigitized:
	case QExiv2::XmpModified:
	default:
		return false;
	}
	return true;
}

// Get a DateTime Tag
// [Original]  Creation date of the intellectual content (e.g. when a photo was taken)
// [Digitized] Creation date of the digital representation (e.g. when an image was digitized)
// [Modified]  Modification date of the digital image file (e.g. when a file was modified by the user)

QDateTime QExiv2::datetime(QExiv2::DateTimeType type) const
{
	QDateTime dt;
	switch (type) {
	case QExiv2::ExifOriginal:
	    dt = QDateTime::fromString(exifTagString("Exif.Photo.DateTimeOriginal"), "yyyy:MM:dd HH:mm:ss");
		if (!dt.isValid()) {
	        dt = QDateTime::fromString(exifTagString("Exif.Image.DateTimeOriginal"), "yyyy:MM:dd HH:mm:ss");
	    }
		break;

	case QExiv2::ExifDigitized:
	    return QDateTime::fromString(exifTagString("Exif.Photo.DateTimeDigitized"), "yyyy:MM:dd HH:mm:ss");

	case QExiv2::ExifModified:
		return QDateTime::fromString(exifTagString("Exif.Image.DateTime"), "yyyy:MM:dd HH:mm:ss");

	case QExiv2::IptcOriginal:
	    // Date represented in the form CCYYMMDD    (Follows ISO 8601 standard)
	    // Time represented in the form HHMMSS:HHMM (Follows ISO 8601 standard)
	    return QDateTime(QDate::fromString(iptcTagString("Iptc.Application2.DateCreated"), Qt::ISODate),
						 QTime::fromString(iptcTagString("Iptc.Application2.TimeCreated"), Qt::ISODate));

	case QExiv2::IptcDigitized:
	    // Date represented in the form CCYYMMDD    (Follows ISO 8601 standard)
	    // Time represented in the form HHMMSS:HHMM (Follows ISO 8601 standard)
	    return QDateTime(QDate::fromString(iptcTagString("Iptc.Application2.DigitizationDate"), Qt::ISODate),
						 QTime::fromString(iptcTagString("Iptc.Application2.DigitizationTime"), Qt::ISODate));

	case QExiv2::XmpOriginal:
	    return QDateTime::fromString(xmpTagString("Xmp.photoshop.DateCreated"), Qt::ISODate);

	case QExiv2::XmpDigitized:
	    return QDateTime::fromString(xmpTagString("Xmp.xmp.CreateDate"), Qt::ISODate);

	case QExiv2::XmpModified:
	    return QDateTime::fromString(xmpTagString("Xmp.xmp.ModifyDate"), Qt::ISODate);

	default:
		return QDateTime();
	}
	return dt;
}
