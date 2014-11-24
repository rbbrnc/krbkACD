#include <QFile>
#include <QDebug>

#include "iso_countries.h"

IsoCountries::IsoCountries()
    : m_WorldRegions {"Africa", "Asia", "Central America", "Europe",
                      "Middle East", "North America", "Oceania",
                      "South America", "The Caribbean"}
{
	load(":/iso3166Africa.txt",     m_AfricaCountriesMap);
	load(":/iso3166Asia.txt",       m_AsiaCountriesMap);
	load(":/iso3166CAmerica.txt",   m_CentralAmericaCountriesMap);
	load(":/iso3166Europe.txt",     m_EuropeCountriesMap);
	load(":/iso3166MiddleEast.txt", m_MiddleEastCountriesMap);
	load(":/iso3166Oceania.txt",    m_OceaniaCountriesMap);
	load(":/iso3166Caribbean.txt",  m_CaribbeanCountriesMap);
	load(":/iso3166NAmerica.txt",   m_NorthAmericaCountriesMap);
	load(":/iso3166SAmerica.txt",   m_SouthAmericaCountriesMap);
}

IsoCountries::~IsoCountries()
{
}

void IsoCountries::load(const QString &countryFileName, QMap<QString, QString> &countryMap)
{
	if (countryFileName.isEmpty()) {
		return;
	}

	QFile file(countryFileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		while (!file.atEnd()) {
			QString line = file.readLine();
			line.remove('\n');
			QStringList sl = line.split(';');
			if (sl.isEmpty() || sl.size() != 2) {
				//qDebug() << __PRETTY_FUNCTION__ << "wrong size";
				continue;
			}
//			qDebug() << sl.at(0) << "---" << sl.at(1);
			countryMap.insert(sl.at(0), sl.at(1));
		}
		file.close();
	}
}


// The magic function, which allows access to the class from anywhere
// To get the value of the instance of the class, call:
// IsoCountries::instance().someMethod();
IsoCountries &IsoCountries::instance()
{
	static IsoCountries instance; // Guaranteed to be destroyed.
	                              // Instantiated on first use.
	return instance;
}

QStringList IsoCountries::worldRegions() const
{
	return m_WorldRegions;
}

QStringList IsoCountries::countryNames(IsoCountries::WorldRegion regionCode) const
{
	switch (regionCode) {
	case IsoCountries::Africa:         return m_AfricaCountriesMap.keys();
	case IsoCountries::Asia:           return m_AsiaCountriesMap.keys();
	case IsoCountries::CentralAmerica: return m_CentralAmericaCountriesMap.keys();
	case IsoCountries::MiddleEast:     return m_MiddleEastCountriesMap.keys();
	case IsoCountries::NorthAmerica:   return m_NorthAmericaCountriesMap.keys();
	case IsoCountries::Oceania:        return m_OceaniaCountriesMap.keys();
	case IsoCountries::SouthAmerica:   return m_SouthAmericaCountriesMap.keys();
	case IsoCountries::TheCaribbean:   return m_CaribbeanCountriesMap.keys();

	case IsoCountries::Europe:
	default:
		return m_EuropeCountriesMap.keys();
	}
}

#if 0
QString IsoCountries::countryCode(const QString &/*countryName*/) const
{
	return QString();
}
#endif

QStringList IsoCountries::countryCodes(IsoCountries::WorldRegion regionCode) const
{
	switch (regionCode) {
	case IsoCountries::Africa:         return m_AfricaCountriesMap.values();
	case IsoCountries::Asia:           return m_AsiaCountriesMap.values();
	case IsoCountries::CentralAmerica: return m_CentralAmericaCountriesMap.values();
	case IsoCountries::MiddleEast:     return m_MiddleEastCountriesMap.values();
	case IsoCountries::NorthAmerica:   return m_NorthAmericaCountriesMap.values();
	case IsoCountries::Oceania:        return m_OceaniaCountriesMap.values();
	case IsoCountries::SouthAmerica:   return m_SouthAmericaCountriesMap.values();
	case IsoCountries::TheCaribbean:   return m_CaribbeanCountriesMap.values();

	case IsoCountries::Europe:
	default:
		return m_EuropeCountriesMap.values();
	}
}
