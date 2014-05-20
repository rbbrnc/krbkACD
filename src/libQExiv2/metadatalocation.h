#ifndef METADATALOCATION_H
#define METADATALOCATION_H

#include <QString>

class MetadataLocation
{
    public:
        MetadataLocation();
//        ~MetadataLocation() {}

        bool isEmpty() const;
        const QString toString() const;

        QString worldRegion;
        QString countryName;
        QString countryCode;
        QString provinceState;
        QString city;
        QString sublocation;
};

#endif
