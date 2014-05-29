#ifndef METADATALOCATION_H
#define METADATALOCATION_H

#include <QString>

class MetadataLocation
{
    public:
        MetadataLocation();
        ~MetadataLocation();

        bool isEmpty() const;
        const QString toString() const;

        void setWorldRegion(const QString &s);
        QString worldRegion() const;

        void setCountryName(const QString &s);
        QString countryName() const;

        void setCountryCode(const QString &s);
        QString countryCode() const;

        void setProvinceState(const QString &s);
        QString provinceState() const;

        void setCity(const QString &s);
        QString city() const;

        void setSublocation(const QString &s);
        QString sublocation() const;

    private:
        QString m_worldRegion;
		QString m_countryName;
		QString m_countryCode;
		QString m_provinceState;
		QString m_city;
		QString m_sublocation;
};

#endif
