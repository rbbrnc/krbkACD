#include "md_location.h"

#include <QDebug>

MetadataLocation::MetadataLocation()
{
}

MetadataLocation::~MetadataLocation()
{
}

void MetadataLocation::setWorldRegion(const QString &s)
{
    m_worldRegion = s;
}

QString MetadataLocation::worldRegion() const
{
    return m_worldRegion;
}

void MetadataLocation::setCountryName(const QString &s)
{
    m_countryName = s;
}

QString MetadataLocation::countryName() const
{
    return m_countryName;
}

void MetadataLocation::setCountryCode(const QString &s)
{
    m_countryCode = s;
}

QString MetadataLocation::countryCode() const
{
    return m_countryCode;
}

void MetadataLocation::setProvinceState(const QString &s)
{
    m_provinceState = s;
}

QString MetadataLocation::provinceState() const
{
    return m_provinceState;
}

void MetadataLocation::setCity(const QString &s)
{
    m_city = s;
}

QString MetadataLocation::city() const
{
    return m_city;
}

void MetadataLocation::setSublocation(const QString &s)
{
    m_sublocation = s;
}

QString MetadataLocation::sublocation() const
{
    return m_sublocation;
}

bool MetadataLocation::isEmpty() const
{
    return (m_worldRegion.isEmpty()
        && m_countryName.isEmpty()
        && m_countryCode.isEmpty()
        && m_provinceState.isEmpty()
        && m_city.isEmpty()
        && m_sublocation.isEmpty());
}

const QString MetadataLocation::toString() const
{
    QString s;

    if (!m_sublocation.isEmpty()) {
        s = m_sublocation + ", ";
    }
    if (!m_city.isEmpty()) {
        s += m_city + ", ";
    }
    if (!m_provinceState.isEmpty()) {
        s += m_provinceState + ", ";
    }
    if (!m_countryName.isEmpty()) {
        s += m_countryName + ", ";
    }
    if (!m_countryCode.isEmpty()) {
        s += m_countryCode + ", ";
    }
    if (!m_worldRegion.isEmpty()) {
        s += m_worldRegion;
    }
    return s;
}
