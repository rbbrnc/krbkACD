// QExiv2 GpsInfo

#include <QDebug>

#include "QExiv2.h"
#include "qexiv2_p.h"

// Match DM + Compass [DDD,MM.mmmmK]
#define WGS84_LATITUDE_DM   "([0-9]|[0-8][0-9]|90),([0-5][0-9]\\.[0-9]*)([NS])"
#define WGS84_LONGITUDE_DM  "([0-9]|[0-9][0-9]|1[0-7][0-9]|180),([0-5][0-9]\\.[0-9]*)([EW])"

// Match DMS + Compass [DDD,MM,SSK]
#define WGS84_LATITUDE_DMS  "([0-9]|[0-8][0-9]|90),([0-5][0-9]),([0-5]|[0-5][0-9])([NS])"
#define WGS84_LONGITUDE_DMS "([0-9]|[0-9][0-9]|1[0-7][0-9]|180),([0-5][0-9]),([0-5]|[0-5][0-9])([EW])"

QGeoCoordinate QExiv2::geoCoordinate() const
{
	// Prefer XMP
	QGeoCoordinate gc = xmpGeoCoordinate();
	if (!gc.isValid()) {
		// Fallback to exif
		gc = exifGeoCoordinate();
	}
	return gc;
}

QGeoCoordinate QExiv2::exifGeoCoordinate() const
{
	double latitude;
	double longitude;

	if (!exifGpsCoordinate("Exif.GPSInfo.GPSLatitude", "Exif.GPSInfo.GPSLatitudeRef", &latitude)) {
		return QGeoCoordinate();
	}

    if (!exifGpsCoordinate("Exif.GPSInfo.GPSLongitude", "Exif.GPSInfo.GPSLongitudeRef", &longitude)) {
		return QGeoCoordinate();
	}

	return QGeoCoordinate(latitude, longitude);
}

QGeoCoordinate QExiv2::xmpGeoCoordinate() const
{
	double latitude;
	double longitude;

    if (!xmpGpsCoordinates(xmpTagString("Xmp.exif.GPSLatitude"), &latitude)) {
		return QGeoCoordinate();
	}

    if (!xmpGpsCoordinates(xmpTagString("Xmp.exif.GPSLongitude"), &longitude)) {
		return QGeoCoordinate();
	}

	return QGeoCoordinate(latitude, longitude);
}

// [Private]
//
// Exif.GPSInfo.GPSLatitudeRef (Ascii)
//    Indicates whether the latitude is north or south latitude.
//    The ASCII value 'N' indicates north latitude, and 'S' is south latitude.
//
// Exif.GpsInfo.GPSLatitude (Rational)
//     Indicates the latitude. The latitude is expressed as three RATIONAL values
//     giving the degrees, minutes, and seconds, respectively.
//     When degrees, minutes and seconds are expressed, the format is dd/1,mm/1,ss/1.
//     When degrees and minutes are used and, for example, fractions of minutes
//     are given up to two decimal places, the format is dd/1,mmmm/100,0/1.
//
// Exif.GPSInfo.GPSLongitudeRef (Ascii)
//    Indicates whether the longitude is east or west longitude.
//    ASCII 'E' indicates east longitude, and 'W' is west longitude.
//
// Exif.GPSInfo.GPSLongitude (Rational)
//    Indicates the longitude. The longitude is expressed as three RATIONAL values
//    giving the degrees, minutes, and seconds, respectively.
//    When degrees, minutes and seconds are expressed, the format is ddd/1,mm/1,ss/1.
//    When degrees and minutes are used and, for example, fractions of minutes are
//    given up to two decimal places, the format is ddd/1,mmmm/100,0/1.
//
bool QExiv2::exifGpsCoordinate(const QString &coordTag, const QString &refTag, double *value) const
{
    const QByteArray ref = exifTagData(refTag.toLatin1().constData());
    if (ref.isEmpty()) {
        return false;
    }

    bool dir = false;
    switch (ref[0]) {
    case 'N':
    case 'E':
        break;
    case 'S':
    case 'W':
        dir = true;
        break;
    default:
        qWarning() << __PRETTY_FUNCTION__ << "Invalid Ref. data:" << ref[0];
        return false;
    }

    double degrees;

    Exiv2::ExifKey  key(coordTag.toLatin1().constData());
    Exiv2::ExifData data(d->exifMetadata);
    Exiv2::ExifData::const_iterator it = data.findKey(key);
    if (it != data.end() && (*it).count() == 3) {
        // Latitude decoding from Exif.
        double num;
        double den;

        // .toRational(long n) Return the n-th component of the value converted
        // to Rational. The return value is -1/1 if the value is not set and the
        // behaviour of the method is undefined if there is no n-th component.
        num = (double)((*it).toRational(0).first);
        den = (double)((*it).toRational(0).second);
        if (den == 0) {
            return false;
        }
        degrees = num/den;

        // Calc minutes
        num = (double)((*it).toRational(1).first);
        den = (double)((*it).toRational(1).second);
        if (den == 0) {
            return false;
        }

        double v;
        v = num/den;
        if (v != -1.0) {
            degrees += v / 60.0;
        }

        // Calc seconds
        num = (double)((*it).toRational(2).first);
        den = (double)((*it).toRational(2).second);
        if (den == 0) {
            // Accept 0/0 seconds.
            if (num == 0) {
                den = 1;
            } else {
                return false;
            }
        }

        v = num/den;
        if (v != -1.0) {
            degrees += v / 3600.0;
        }

        if (dir) {
            degrees *= -1.0;
        }
        *value = degrees;
        return true;
    }

    return false;
}

// [Private]
//
// Exif.GPSInfo.GPSAltitudeRef (Byte)
//    Indicates the altitude used as the reference altitude.
//    If the reference is sea level and the altitude is above sea level, 0 is given.
//    If the altitude is below sea level, a value of 1 is given and the altitude
//    is indicated as an absolute value in the GSPAltitude tag.
//    The reference unit is meters.
//    Note that this tag is BYTE type, unlike other reference tags.
//
// Exif.GPSInfo.GPSAltitude (Rational)
//    Indicates the altitude based on the reference in GPSAltitudeRef.
//    Altitude is expressed as one RATIONAL value. The reference unit is meters.
//
#if 0
bool QExiv2::exifGpsAltitude(double *altitude) const
{
	*altitude = 0.0;
    return true;
}

bool QExiv2::xmpGpsAltitude(double *altitude) const
{
#if 0
    const QString ref= getXmpTagString("Xmp.exif.GPSAltitudeRef");
    if (ref.isEmpty()) {
        return false;
    }
    const QString alt = getXmpTagString("Xmp.exif.GPSAltitude");
    if (alt.isEmpty()) {
        return false;
    }

    if (ref == QString("1")) {
        *altitude *= -1.0;
    }
#else
    *altitude = 0.0;
    return true;
#endif
}
#endif


// GPSTimeStamp
//
// Time stamp of GPS data, in Coordinated Universal Time.
// Note: The GPSDateStamp tag is new in EXIF 2.2.
//       The GPS timestamp in EXIF 2.1 does not include a date.
//
// If not present, the date component for the XMP should be taken from
// exif:DateTimeOriginal, or if that is also lacking from exif:DateTimeDigitized.
// If no date is available, do not write exif:GPSTimeStamp to XMP.
//
// Exif.GPSInfo.GPSTimeStamp (Rational)
//    Indicates the time as UTC (Coordinated Universal Time).
//    <TimeStamp> is expressed as three RATIONAL values giving the
//    hour, minute, and second (atomic clock).
//
QDateTime QExiv2::gpsTimestamp() const
{
    qDebug() << xmpTagString("Xmp.exif.GPSTimeStamp");
    return QDateTime();
}

double QExiv2::gpsCoordinates(const double deg, const double min, const double sec, const QChar &dir) const
{
     double degree = deg + (min / 60.0) + (sec / 3600.0);

     if ((dir == 'W') || (dir == 'S')) {
         degree *= -1.0;
     }
     return degree;
}

bool QExiv2::xmpGpsCoordinates(const QString &str, double *value) const
{
     if (str.isEmpty()) {
         return false;
     }

     QString s = str.simplified().toUpper();
     QRegExp r;

     r.setPattern(WGS84_LATITUDE_DM);
     if (r.exactMatch(s)) {
         r.indexIn(s);
         *value = gpsCoordinates(r.cap(1).toDouble(), r.cap(2).toDouble(), 0.0, r.cap(3).at(0));
         return true;
     }

     r.setPattern(WGS84_LONGITUDE_DM);
     if (r.exactMatch(s)) {
         r.indexIn(s);
         *value = gpsCoordinates(r.cap(1).toDouble(), r.cap(2).toDouble(), 0.0, r.cap(3).at(0));
         return true;
     }

     r.setPattern(WGS84_LATITUDE_DMS);
     if (r.exactMatch(s)) {
         r.indexIn(s);
         *value = gpsCoordinates(r.cap(1).toDouble(), r.cap(2).toDouble(), r.cap(3).toDouble(), r.cap(4).at(0));
         return true;
     }

     r.setPattern(WGS84_LONGITUDE_DMS);
     if (r.exactMatch(s)) {
         r.indexIn(s);
         *value = gpsCoordinates(r.cap(1).toDouble(), r.cap(2).toDouble(), r.cap(3).toDouble(), r.cap(4).at(0));
         return true;
     }
    return false;
}
