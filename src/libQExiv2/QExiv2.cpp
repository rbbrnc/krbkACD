#include <QFile>
#include <QFileInfo>
#include <QDebug>

#include "QExiv2.h"
#include "QExiv2_p.h"

///////////////////////////////////////////////////////////////////////////////
/// class QExiv2
///////////////////////////////////////////////////////////////////////////////
QExiv2::QExiv2() : d(new QExiv2DataPrivate)
{
}

QExiv2::QExiv2(const QExiv2& other) : d(other.d)
{
}

QExiv2::QExiv2(const QString& filePath) : d(new QExiv2DataPrivate)
{
	load(filePath);
}

QExiv2::~QExiv2()
{
    Exiv2::XmpProperties::unregisterNs();
}

QExiv2& QExiv2::operator=(const QExiv2& other)
{
	d = other.d;
	return *this;
}

bool QExiv2::isValid() const
{
	return d->metadataValid;
}

bool QExiv2::hasExif() const
{
	return !d->exifMetadata.empty();
}

bool QExiv2::hasIptc() const
{
	return !d->iptcMetadata.empty();
}

bool QExiv2::hasXmp() const
{
	return !d->xmpMetadata.empty();
}

bool QExiv2::hasComment() const
{
	return !d->imageComment.empty();
}

#if 0
bool QExiv2::loadFromData(const QByteArray& data)
{
	if (data.isEmpty()) {
		return false;
	}

	try {
		d->image = Exiv2::ImageFactory::open((Exiv2::byte *) data.constData(), data.size());
	} catch (Exiv2::Error &e) {
		d->error(__PRETTY_FUNCTION__, e);
		return false;
	}

	return d->readMetadata();
}
#endif

bool QExiv2::load(const QString& filePath)
{
	if (filePath.isEmpty()) {
		return false;
	}

	// Check if file exists and is readable!!!!
	QFileInfo info(filePath);
	if (!info.isReadable()) {
		qDebug() << "File '" << info.filePath().toLatin1().constData() << "' is not readable.";
		return false;
	}
    // Don't read directories
    if (info.isDir()) {
        return false;
    }

	try {
		d->image = Exiv2::ImageFactory::open((const char *)(QFile::encodeName(filePath)));
	} catch (Exiv2::Error &e) {
		d->error(__PRETTY_FUNCTION__, e);
		return false;
	}

	return d->readMetadata();
}

// Se il file non ha i permessi in scrittura scatta l'exception
bool QExiv2::save()
{
	if (!isValid()) {
		return false;
	}

	try {
		bool update = false;
		if (isImgCommentWritable()) {
			if (d->image->comment() != d->imageComment) {
				d->image->setComment(d->imageComment);
				update = true;
			}
        }

		if (isXmpWritable()) {
			d->image->setXmpData(d->xmpMetadata);
			update = true;
        } else {
            qDebug() << __PRETTY_FUNCTION__ << "XMP Not writable";
        }

		if (update) {
//			qDebug() << __PRETTY_FUNCTION__ << "write Metadata";
			d->image->writeMetadata();
		}

        return true;

	} catch (Exiv2::Error& e) {
		d->error(__PRETTY_FUNCTION__, e);
	}

	return false;
}

// Utility functions
void QExiv2::locationCreated(MetadataLocation &loc, int index)
{
    // ===================================================================
    // Get Location Created tags
    //
    // This information describes the location where the image was created,
    // the location of the camera during shot creation.
    // The typical case is when a GPS receiver injects the current location
    // into an image at shooting time (camera location).
    //
    // 1st try Xmp.iptcExt tags
    //
    // Xmp.iptcExt.LocationCreated[1]/Iptc4xmpExt:WorldRegion
    // Xmp.iptcExt.LocationCreated[1]/Iptc4xmpExt:CountryName
    // Xmp.iptcExt.LocationCreated[1]/Iptc4xmpExt:CountryCode
    // Xmp.iptcExt.LocationCreated[1]/Iptc4xmpExt:City
    // Xmp.iptcExt.LocationCreated[1]/Iptc4xmpExt:ProvinceState
    // Xmp.iptcExt.LocationCreated[1]/Iptc4xmpExt:Sublocation
    //
    // 2nd Try Xmp.photoshop and Iptc.Application2 tags
    //
    // Xmp.photoshop.Country _or_  Iptc.Application2.CountryName
    // ---                         Iptc.Application2.CountryCode
    // Xmp.photoshop.State   _or_  Iptc.Application2.ProvinceState
    // Xmp.photoshop.City    _or_  Iptc.Application2.City
    // ---                         Iptc.Application2.SubLocation
    //
    // ===================================================================
    QString xmpPath = QString("Xmp.iptcExt.LocationCreated[%1]/Iptc4xmpExt:").arg(index);

    loc.worldRegion = xmpTagString(xmpPath + "WorldRegion", true);
    loc.countryName = xmpTagString(xmpPath + "CountryName", true);
    if (loc.countryName.isEmpty()) {
        loc.countryName = xmpTagString("Xmp.photoshop.Country", true);
        if (loc.countryName.isEmpty()) {
            loc.countryName = iptcTagString("Iptc.Application2.CountryName", true);
        }
    }

    loc.countryCode = xmpTagString(xmpPath + "CountryCode", true);
    if (loc.countryCode.isEmpty()) {
        loc.countryCode = iptcTagString("Iptc.Application2.CountryCode", true);
    }

    loc.provinceState = xmpTagString(xmpPath + "ProvinceState", true);
    if (loc.provinceState.isEmpty()) {
        loc.provinceState = xmpTagString("Xmp.photoshop.State", true);
        if (loc.provinceState.isEmpty()) {
            loc.provinceState = iptcTagString("Iptc.Application2.ProvinceState", true);
        }
    }

    loc.city = xmpTagString(xmpPath + "City", true);
    if (loc.city.isEmpty()) {
        loc.city = xmpTagString("Xmp.photoshop.City", true);
        if (loc.city.isEmpty()) {
            loc.city = iptcTagString("Iptc.Application2.City", true);
        }
    }

    loc.sublocation = xmpTagString(xmpPath + "Sublocation", true);
    if (loc.sublocation.isEmpty()) {
        loc.sublocation = iptcTagString("Iptc.Application2.SubLocation", true);
    }
}

void QExiv2::locationShown(MetadataLocation &loc, int index)
{
    // ===================================================================
    // Get Location Shown tags
    //
    // This information describes the location of the main subject being
    // shown in an image. For example, a picture of Mount Fuji would be
    // tagged with the coordinates of where the mountain is located
    // (subject location), although the picture may have been taken from
    // downtown Tokyo.
    //
    // 1st try Xmp.iptcExt tags
    //
    // Xmp.iptcExt.LocationShown[1]/Iptc4xmpExt:WorldRegion
    // Xmp.iptcExt.LocationShown[1]/Iptc4xmpExt:CountryName
    // Xmp.iptcExt.LocationShown[1]/Iptc4xmpExt:CountryCode
    // Xmp.iptcExt.LocationShown[1]/Iptc4xmpExt:ProvinceState
    // Xmp.iptcExt.LocationShown[1]/Iptc4xmpExt:City
    // Xmp.iptcExt.LocationShown[1]/Iptc4xmpExt:Sublocation
    //
    // 2nd try legacy Iptc.Application2 tags
    //
    // Iptc.Application2.LocationName
    // Iptc.Application2.LocationCode
    //
    // ====================================================================
    QString xmpPath = QString("Xmp.iptcExt.LocationShown[%1]/Iptc4xmpExt:").arg(index);

    loc.worldRegion = xmpTagString(xmpPath + "WorldRegion", true);
    loc.countryName = xmpTagString(xmpPath + "CountryName", true);
    if (loc.countryName.isEmpty()) {
        loc.countryName = iptcTagString("Iptc.Application2.LocationName", true);
    }

    loc.countryCode = xmpTagString(xmpPath + "CountryCode", true);
    if (loc.countryCode.isEmpty()) {
        loc.countryCode = iptcTagString("Iptc.Application2.LocationCode", true);
    }

    loc.provinceState = xmpTagString(xmpPath + "ProvinceState", true);
    loc.city          = xmpTagString(xmpPath + "City", true);
    loc.sublocation   = xmpTagString(xmpPath + "Sublocation", true);
}

void QExiv2::setLocationShown(MetadataLocation &loc, int index)
{
    if (index != 1) {
        #warning TODO
        qWarning() << __PRETTY_FUNCTION__ << "index >1 not implemented yet!";
        return;
    }

    QString xmpPath = QString("Xmp.iptcExt.LocationShown[%1]/Iptc4xmpExt:").arg(index);

    if (loc.isEmpty()) {
        removeXmpBag("Xmp.iptcExt.LocationShown", 25);
    } else {
        if (xmpTagString("Xmp.iptcExt.LocationShown").isNull()) {
            setXmpTagBag("Xmp.iptcExt.LocationShown");
        }
        setXmpTagString(xmpPath + "WorldRegion", loc.worldRegion);
        setXmpTagString(xmpPath + "CountryName", loc.countryName);
        setXmpTagString(xmpPath + "CountryCode", loc.countryCode);
        setXmpTagString(xmpPath + "ProvinceState", loc.provinceState);
        setXmpTagString(xmpPath + "City", loc.city);
        setXmpTagString(xmpPath + "Sublocation", loc.sublocation);
    }
}

void QExiv2::setLocationCreated(MetadataLocation &loc, int index)
{
    if (index != 1) {
        #warning TODO
        qWarning() << __PRETTY_FUNCTION__ << "index >1 not implemented yet!";
        return;
    }

    QString xmpPath = QString("Xmp.iptcExt.LocationCreated[%1]/Iptc4xmpExt:").arg(index);

    if (loc.isEmpty()) {
        removeXmpBag("Xmp.iptcExt.LocationCreated", 27);
    } else {
        if (xmpTagString("Xmp.iptcExt.LocationCreated").isNull()) {
            setXmpTagBag("Xmp.iptcExt.LocationCreated");
        }
        setXmpTagString(xmpPath + "WorldRegion", loc.worldRegion);
        setXmpTagString(xmpPath + "CountryName", loc.countryName);
        setXmpTagString(xmpPath + "CountryCode", loc.countryCode);
        setXmpTagString(xmpPath + "ProvinceState", loc.provinceState);
        setXmpTagString(xmpPath + "City", loc.city);
        setXmpTagString(xmpPath + "Sublocation", loc.sublocation);
    }
}

// GPS DATA Utilities

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

}

// Match DM + Compass [DDD,MM.mmmmK]
#define WGS84_LATITUDE_DM   "([0-9]|[0-8][0-9]|90),([0-5][0-9]\\.[0-9]*)([NS])"
#define WGS84_LONGITUDE_DM  "([0-9]|[0-9][0-9]|1[0-7][0-9]|180),([0-5][0-9]\\.[0-9]*)([EW])"

// Match DMS + Compass [DDD,MM,SSK]
#define WGS84_LATITUDE_DMS  "([0-9]|[0-8][0-9]|90),([0-5][0-9]),([0-5]|[0-5][0-9])([NS])"
#define WGS84_LONGITUDE_DMS "([0-9]|[0-9][0-9]|1[0-7][0-9]|180),([0-5][0-9]),([0-5]|[0-5][0-9])([EW])"

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
