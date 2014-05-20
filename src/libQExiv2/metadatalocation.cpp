///////////////////////////////////////////////////////////////////////////////
/// class Metadata Location
///////////////////////////////////////////////////////////////////////////////

#include "metadatalocation.h"

MetadataLocation::MetadataLocation()
{
}

bool MetadataLocation::isEmpty() const
{
    return (worldRegion.isEmpty()
        && countryName.isEmpty()
        && countryCode.isEmpty()
        && provinceState.isEmpty()
        && city.isEmpty()
        && sublocation.isEmpty());
}

const QString MetadataLocation::toString() const
{
    QString s;

    if (!sublocation.isEmpty()) {
        s = sublocation + ", ";
    }
    if (!city.isEmpty()) {
        s += city + ", ";
    }
    if (!provinceState.isEmpty()) {
        s += provinceState + ", ";
    }
    if (!countryName.isEmpty()) {
        s += countryName + ", ";
    }
    if (!countryCode.isEmpty()) {
        s += countryCode + ", ";
    }
    if (!worldRegion.isEmpty()) {
        s += worldRegion;
    }

    return s;
}
