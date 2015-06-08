#include "QExiv2.h"
#include "qexiv2_p.h"

// [Private]
void QExiv2::setLocation(MetadataLocation &loc, const QString &locType, int index)
{
    if (index != 1) {
        qWarning() << Q_FUNC_INFO << "index >1 not implemented yet!";
        return;
    }

	QString xmpBase = QString("Xmp.iptcExt.%1").arg(locType);
    QString xmpPath = QString("%1[%2]/Iptc4xmpExt:").arg(xmpBase).arg(index);

    if (loc.isEmpty()) {
        removeXmpBag(xmpBase.toLatin1().constData(), xmpBase.size());
    } else {
        if (xmpTagString(xmpBase).isNull()) {
            setXmpTagBag(xmpBase.toLatin1().constData());
        }
        setXmpTagString(xmpPath + "WorldRegion", loc.worldRegion());
        setXmpTagString(xmpPath + "CountryName", loc.countryName());
        setXmpTagString(xmpPath + "CountryCode", loc.countryCode());
        setXmpTagString(xmpPath + "ProvinceState", loc.provinceState());
        setXmpTagString(xmpPath + "City", loc.city());
        setXmpTagString(xmpPath + "Sublocation", loc.sublocation());
    }
}

// [Private]
void QExiv2::location(MetadataLocation &loc, const QString &locType, int index)
{
	QString xmpBase = QString("Xmp.iptcExt.%1").arg(locType);
    QString xmpPath = QString("%1[%2]/Iptc4xmpExt:").arg(xmpBase).arg(index);

    loc.setWorldRegion(xmpTagString(xmpPath + "WorldRegion", true));
	loc.setCountryName(xmpTagString(xmpPath + "CountryName", true));
	loc.setCountryCode(xmpTagString(xmpPath + "CountryCode", true));
	loc.setProvinceState(xmpTagString(xmpPath + "ProvinceState", true));
    loc.setCity(xmpTagString(xmpPath + "City", true));
    loc.setSublocation(xmpTagString(xmpPath + "Sublocation", true));
}

void QExiv2::setLocationShown(MetadataLocation &loc, int index)
{
	setLocation(loc, "LocationShown", index);
}

void QExiv2::setLocationCreated(MetadataLocation &loc, int index)
{
	setLocation(loc, "LocationCreated", index);
}

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
void QExiv2::locationCreated(MetadataLocation &loc, int index)
{
	// Get xmp.iptcExt tags
	location(loc, "LocationCreated", index);

    if (loc.countryName().isEmpty()) {
        loc.setCountryName(xmpTagString("Xmp.photoshop.Country", true));
        if (loc.countryName().isEmpty()) {
            loc.setCountryName(iptcTagString("Iptc.Application2.CountryName", true));
        }
    }

    if (loc.countryCode().isEmpty()) {
        loc.setCountryCode(iptcTagString("Iptc.Application2.CountryCode", true));
    }

    if (loc.provinceState().isEmpty()) {
        loc.setProvinceState(xmpTagString("Xmp.photoshop.State", true));
        if (loc.provinceState().isEmpty()) {
            loc.setProvinceState(iptcTagString("Iptc.Application2.ProvinceState", true));
        }
    }

    if (loc.city().isEmpty()) {
        loc.setCity(xmpTagString("Xmp.photoshop.City", true));
        if (loc.city().isEmpty()) {
            loc.setCity(iptcTagString("Iptc.Application2.City", true));
        }
    }

    if (loc.sublocation().isEmpty()) {
        loc.setSublocation(iptcTagString("Iptc.Application2.SubLocation", true));
    }
}

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
void QExiv2::locationShown(MetadataLocation &loc, int index)
{
	// Get xmp.iptcExt tags
	location(loc, "LocationShown", index);
	if (loc.countryName().isEmpty()) {
        loc.setCountryName(iptcTagString("Iptc.Application2.LocationName", true));
	}
    if (loc.countryCode().isEmpty()) {
        loc.setCountryCode(iptcTagString("Iptc.Application2.LocationCode", true));
    }
}
