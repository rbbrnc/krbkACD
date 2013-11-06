#ifndef ISO_COUNTRIES_H
#define ISO_COUNTRIES_H

#include <QStringList>
#include <QMap>

class IsoCountries
{
	private:
		IsoCountries();
		static IsoCountries *m_instance;

		void load(const QString &countryFileName, QMap<QString, QString> &countryMap);

	public:
		static IsoCountries *instance();
		~IsoCountries();

		enum WorldRegion {
			Africa = 0,
			Asia,
			CentralAmerica,
			Europe,
			MiddleEast,
			NorthAmerica,
			Oceania,
			SouthAmerica,
			TheCaribbean
		};

		QStringList worldRegions() const;
		QStringList countryNames(IsoCountries::WorldRegion regionCode) const;
//		QString countryCode(const QString &countryName) const;
		QStringList countryCodes(IsoCountries::WorldRegion regionCode) const;

	private:
		QStringList m_WorldRegions;

		QMap <QString, QString> m_EuropeCountriesMap;
		QMap <QString, QString> m_OceaniaCountriesMap;
		QMap <QString, QString> m_CaribbeanCountriesMap;
		QMap <QString, QString> m_AfricaCountriesMap;
		QMap <QString, QString> m_NorthAmericaCountriesMap;
		QMap <QString, QString> m_CentralAmericaCountriesMap;
		QMap <QString, QString> m_SouthAmericaCountriesMap;
		QMap <QString, QString> m_AsiaCountriesMap;
		QMap <QString, QString> m_MiddleEastCountriesMap;

};

#endif
