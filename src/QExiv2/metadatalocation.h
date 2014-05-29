#ifndef METADATALOCATION_H
#define METADATALOCATION_H

#include <QString>
#include <QGeoLocation>

class MetadataLocation
{
    public:
        MetadataLocation();
//        ~MetadataLocation() {}

        bool isEmpty() const;
        const QString toString() const;

        void setWorldRegion(const QString &wRegion);
        QString worldRegion_2() const;

        QString worldRegion;
        QString countryName;
        QString countryCode;
        QString provinceState;
        QString city;
        QString sublocation;

    private:
        QString m_worldRegion;
        QGeoLocation m_location;
};

#endif
