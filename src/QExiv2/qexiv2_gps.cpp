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


void QExiv2::geoCoordinate(QGeoCoordinate *gc) const
{
    bool   ok;
    double lat;
    double lon;

    lat = gpsCoordinatesFromString(xmpTagString("Xmp.exif.GPSLatitude"), &ok);
    if (ok) {
        gc->setLatitude(lat);
    } else {
        qDebug() << "eLat:"
                 << exifTagString("Exif.GPSInfo.GPSLatitude")
                 << exifTagString("Exif.GPSInfo.GPSLatitudeRef");
    }

    lon = gpsCoordinatesFromString(xmpTagString("Xmp.exif.GPSLongitude"), &ok);
    if (ok) {
        gc->setLongitude(lon);
    } else {
        qDebug() << "eLon:"
                 << exifTagString("Exif.GPSInfo.GPSLongitude")
                 << exifTagString("Exif.GPSInfo.GPSLongitudeRef");
    }
    //    QByteArray ba = m_exiv2->exifTagData("Exif.GPSInfo.GPSLatitude");
    //    qDebug() << "BA:" << ba.toHex() << "___";



    //GPSAltitudeRef GPS Altitude   Reference 	Closed Choice of Integer 	XmpText 	Internal 	GPS tag 5, 0x05. Indicates whether the altitude is above or below sea level.
    //GPSAltitude 	 GPS Altitude 	Rational 	XmpText 	Internal 	GPS tag 6, 0x06. Indicates altitude in meters.
    //GPSTimeStamp 	 GPS Time Stamp Date 	    XmpText 	Internal 	GPS tag 29 (date), 0x1D, and, and GPS tag 7 (time), 0x07. Time stamp of GPS data, in Coordinated Universal Time.
                     //Note: The GPSDateStamp tag is new in EXIF 2.2.
                     //The GPS timestamp in EXIF 2.1 does not include a date.
                     //If not present, the date component for the XMP should be taken from exif:DateTimeOriginal,
                     //or if that is also lacking from exif:DateTimeDigitized.
                     //If no date is available, do not write exif:GPSTimeStamp to XMP.
}

double QExiv2::gpsCoordinates(const double deg, const double min, const double sec, const QChar &dir) const
{
     double degree = deg + (min / 60.0) + (sec / 3600.0);

     if ((dir == 'W') || (dir == 'S')) {
         degree *= -1.0;
     }
     return degree;
}

double QExiv2::gpsCoordinatesFromString(const QString &str, bool *ok) const
{
     if (str.isEmpty()) {
         *ok = false;
         return 0.0;
     }

     QString s = str.simplified().toUpper();

     *ok = true;
     QRegExp r;

     r.setPattern(WGS84_LATITUDE_DM);
     if (r.exactMatch(s)) {
         r.indexIn(s);
         return gpsCoordinates(r.cap(1).toDouble(), r.cap(2).toDouble(),
                               0.0, r.cap(3).at(0));
     }

     r.setPattern(WGS84_LONGITUDE_DM);
     if (r.exactMatch(s)) {
         r.indexIn(s);
         return gpsCoordinates(r.cap(1).toDouble(), r.cap(2).toDouble(),
                               0.0, r.cap(3).at(0));
     }

     r.setPattern(WGS84_LATITUDE_DMS);
     if (r.exactMatch(s)) {
         r.indexIn(s);
         return gpsCoordinates(r.cap(1).toDouble(), r.cap(2).toDouble(),
                               r.cap(3).toDouble(), r.cap(4).at(0));
     }

     r.setPattern(WGS84_LONGITUDE_DMS);
     if (r.exactMatch(s)) {
         r.indexIn(s);
         return gpsCoordinates(r.cap(1).toDouble(), r.cap(2).toDouble(),
                               r.cap(3).toDouble(), r.cap(4).at(0));
     }

     *ok = false;
     return 0.0;
}
