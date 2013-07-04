#include "iso_countries.h"

IsoCountries *IsoCountries::m_instance = 0;

IsoCountries::IsoCountries()
{
	m_WorldRegions << "Africa"
		       << "Asia"
		       << "Central America"
		       << "Europe"
		       << "Middle East"
		       << "North America"
		       << "Oceania"
		       << "South America"
		       << "The Caribbean";

	m_AfricaCountriesMap.insert("Algeria", "DZA");
	m_AfricaCountriesMap.insert("Angola", "AGO");
	m_AfricaCountriesMap.insert("Benin", "BEN");
	m_AfricaCountriesMap.insert("Botswana", "BWA");
	m_AfricaCountriesMap.insert("Burkina Faso", "BFA");
	m_AfricaCountriesMap.insert("Burundi", "BDI");
	m_AfricaCountriesMap.insert("Cameroon", "CMR");
	m_AfricaCountriesMap.insert("Cape Verde", "CPV");
	m_AfricaCountriesMap.insert("Central African Republic", "CAF");
	m_AfricaCountriesMap.insert("Chad", "TCD");
	m_AfricaCountriesMap.insert("Comoros", "COM");
	m_AfricaCountriesMap.insert("Congo, Democratic Republic of", "COD");
	m_AfricaCountriesMap.insert("Congo, People's Republic of", "COG");
	m_AfricaCountriesMap.insert("Cote d'Ivoire", "CIV");
	m_AfricaCountriesMap.insert("Djibouti", "DJI");
	m_AfricaCountriesMap.insert("Egypt", "EGY");
	m_AfricaCountriesMap.insert("Equatorial Guinea", "GNQ");
	m_AfricaCountriesMap.insert("Eritrea", "ERI");
	m_AfricaCountriesMap.insert("Ethiopia", "ETH");
	m_AfricaCountriesMap.insert("Gabon", "GAB");
	m_AfricaCountriesMap.insert("Gambia", "GMB");
	m_AfricaCountriesMap.insert("Ghana", "GHA");
	m_AfricaCountriesMap.insert("Guinea", "GIN");
	m_AfricaCountriesMap.insert("Guinea-Bissau", "GNB");
	m_AfricaCountriesMap.insert("Kenya", "KEN");
	m_AfricaCountriesMap.insert("Lesotho", "LSO");
	m_AfricaCountriesMap.insert("Liberia", "LBR");
	m_AfricaCountriesMap.insert("Libyan Arab Jamahiriya", "LBY");
	m_AfricaCountriesMap.insert("Madagascar", "MDG");
	m_AfricaCountriesMap.insert("Malawi", "MWI");
	m_AfricaCountriesMap.insert("Mali", "MLI");
	m_AfricaCountriesMap.insert("Mauritania", "MRT");
	m_AfricaCountriesMap.insert("Mauritius", "MUS");
	m_AfricaCountriesMap.insert("Mayotte", "MYT");
	m_AfricaCountriesMap.insert("Morocco", "MAR");
	m_AfricaCountriesMap.insert("Mozambique", "MOZ");
	m_AfricaCountriesMap.insert("Namibia",    "NAM");
	m_AfricaCountriesMap.insert("Niger",      "NER");
	m_AfricaCountriesMap.insert("Nigeria",    "NGA");
	m_AfricaCountriesMap.insert("Reunion",    "REU");
	m_AfricaCountriesMap.insert("Rwanda",     "RWA");
	m_AfricaCountriesMap.insert("St. Helena",   "SHN");
	m_AfricaCountriesMap.insert("Sao Tome and Principe", "STP");
	m_AfricaCountriesMap.insert("Senegal",      "SEN");
	m_AfricaCountriesMap.insert("Seychelles",   "SYC");
	m_AfricaCountriesMap.insert("Sierra Leone", "SLE");
	m_AfricaCountriesMap.insert("Somalia",      "SOM");
	m_AfricaCountriesMap.insert("South Africa", "ZAF");
	m_AfricaCountriesMap.insert("Sudan",        "SDN");
	m_AfricaCountriesMap.insert("Swaziland", "SWZ");
	m_AfricaCountriesMap.insert("Tanzania", "TZA");
	m_AfricaCountriesMap.insert("Togo", "TGO");
	m_AfricaCountriesMap.insert("Tunisia", "TUN");
	m_AfricaCountriesMap.insert("Uganda", "UGA");
	m_AfricaCountriesMap.insert("Western Sahara", "ESH");
	m_AfricaCountriesMap.insert("Zambia", "ZMB");
	m_AfricaCountriesMap.insert("Zimbabwe", "ZWE");

	m_NorthAmericaCountriesMap.insert("Bermuda",   "BMU");
	m_NorthAmericaCountriesMap.insert("Canada",    "CAN");
	m_NorthAmericaCountriesMap.insert("Greenland", "GRL");
	m_NorthAmericaCountriesMap.insert("Saint Pierre and Miquelon", "SPM");
	m_NorthAmericaCountriesMap.insert("United States", "USA");

	m_CentralAmericaCountriesMap.insert("Belize",      "BLZ");
	m_CentralAmericaCountriesMap.insert("Costa Rica",  "CRI");
	m_CentralAmericaCountriesMap.insert("El Salvador", "SLV");
	m_CentralAmericaCountriesMap.insert("Guatemala",   "GTM");
	m_CentralAmericaCountriesMap.insert("Honduras",    "HND");
	m_CentralAmericaCountriesMap.insert("Mexico",      "MEX");
	m_CentralAmericaCountriesMap.insert("Nicaragua",   "NIC");
	m_CentralAmericaCountriesMap.insert("Panama",      "PAN");

	m_SouthAmericaCountriesMap.insert("Argentina", "ARG");
	m_SouthAmericaCountriesMap.insert("Bolivia",   "BOL");
	m_SouthAmericaCountriesMap.insert("Brazil",    "BRA");
	m_SouthAmericaCountriesMap.insert("Chile",     "CHL");
	m_SouthAmericaCountriesMap.insert("Colombia",  "COL");
	m_SouthAmericaCountriesMap.insert("Ecuador",   "ECU");
	m_SouthAmericaCountriesMap.insert("Falkland Islands", "FLK");
	m_SouthAmericaCountriesMap.insert("French Guiana", "GUF");
	m_SouthAmericaCountriesMap.insert("Guyana",    "GUY");
	m_SouthAmericaCountriesMap.insert("Paraguay",  "PRY");
	m_SouthAmericaCountriesMap.insert("Peru",      "PER");
	m_SouthAmericaCountriesMap.insert("Suriname",  "SUR");
	m_SouthAmericaCountriesMap.insert("Uruguay",   "URY");
	m_SouthAmericaCountriesMap.insert("Venezuela", "VEN");

	m_AsiaCountriesMap.insert("Afganistan", "AFG");
	m_AsiaCountriesMap.insert("Armenia", "ARM");
	m_AsiaCountriesMap.insert("Azerbaijan", "AZE");
	m_AsiaCountriesMap.insert("Bangladesh", "BGD");
	m_AsiaCountriesMap.insert("Bhutan", "BTN");
	m_AsiaCountriesMap.insert("Brunei Darussalam", "BRN");
	m_AsiaCountriesMap.insert("Cambodia", "KHM");
	m_AsiaCountriesMap.insert("China", "CHN");
	m_AsiaCountriesMap.insert("Georgia", "GEO");
	m_AsiaCountriesMap.insert("Hong Kong", "HKG");
	m_AsiaCountriesMap.insert("India", "IND");
	m_AsiaCountriesMap.insert("Indonesia", "IDN");
	m_AsiaCountriesMap.insert("Japan", "JPN");
	m_AsiaCountriesMap.insert("Kazakhstan", "KAZ");
	m_AsiaCountriesMap.insert("Korea, North", "PRK");
	m_AsiaCountriesMap.insert("Korea, South", "KOR");
//	m_AsiaCountriesMap.insert("Kyrgyzstan", "");
	m_AsiaCountriesMap.insert("Laos", "LAO");
	m_AsiaCountriesMap.insert("Macao", "MAC");
	m_AsiaCountriesMap.insert("Malaysia", "MYS");
	m_AsiaCountriesMap.insert("Maldives", "MDV");
	m_AsiaCountriesMap.insert("Mongolia", "MNG");
	m_AsiaCountriesMap.insert("Myanmar", "MMR");
	m_AsiaCountriesMap.insert("Nepal", "NPL");
	m_AsiaCountriesMap.insert("Pakistan", "PAK");
	m_AsiaCountriesMap.insert("Philippines", "PHL");
	m_AsiaCountriesMap.insert("Singapore", "SGP");
	m_AsiaCountriesMap.insert("Sri Lanka", "LKA");
	m_AsiaCountriesMap.insert("Taiwan", "TWN");
	m_AsiaCountriesMap.insert("Tajikistan", "TJK");
	m_AsiaCountriesMap.insert("Thailand", "THA");
	m_AsiaCountriesMap.insert("Timor Leste (West)", "TLS");
	m_AsiaCountriesMap.insert("Turkmenistan", "TKM");
	m_AsiaCountriesMap.insert("Uzbekistan", "UZB");
	m_AsiaCountriesMap.insert("Vietnam", "VNM");

	m_EuropeCountriesMap.insert("Albania", "ALB");
	m_EuropeCountriesMap.insert("Andorra", "AND");
	m_EuropeCountriesMap.insert("Austria", "AUT");
	m_EuropeCountriesMap.insert("Belarus", "BLR");
	m_EuropeCountriesMap.insert("Belgium", "BEL");
	m_EuropeCountriesMap.insert("Bosnia and Herzegovina", "BIH");
	m_EuropeCountriesMap.insert("Bulgaria", "BGR");
	m_EuropeCountriesMap.insert("Croatia", "HRV");
	m_EuropeCountriesMap.insert("Cyprus", "CYP");
	m_EuropeCountriesMap.insert("Czech Republic", "CZE");
	m_EuropeCountriesMap.insert("Denmark", "DNK");
	m_EuropeCountriesMap.insert("Estonia", "EST");
	m_EuropeCountriesMap.insert("Faroe Islands", "FRO");
	m_EuropeCountriesMap.insert("Finland", "FIN");
	m_EuropeCountriesMap.insert("France", "FRA");
	m_EuropeCountriesMap.insert("Germany", "DEU");
	m_EuropeCountriesMap.insert("Gibraltar", "GIB");
	m_EuropeCountriesMap.insert("Greece", "GRC");
//	m_EuropeCountriesMap.insert("Guerney and Alderney", "");
	m_EuropeCountriesMap.insert("Hungary", "HUN");
	m_EuropeCountriesMap.insert("Iceland", "ISL");
	m_EuropeCountriesMap.insert("Ireland", "IRL");
	m_EuropeCountriesMap.insert("Italy", "ITA");
//	m_EuropeCountriesMap.insert("Jersey", "");
//	m_EuropeCountriesMap.insert("Kosovo", "");
	m_EuropeCountriesMap.insert("Latvia", "LVA");
	m_EuropeCountriesMap.insert("Liechtenstein", "LIE");
	m_EuropeCountriesMap.insert("Lithuania", "LTU");
	m_EuropeCountriesMap.insert("Luxembourg", "LUX");
	m_EuropeCountriesMap.insert("Macedonia", "MKD");
	m_EuropeCountriesMap.insert("Malta", "MLT");
//	m_EuropeCountriesMap.insert("Man, Island of", "");
	m_EuropeCountriesMap.insert("Moldova", "MDA");
	m_EuropeCountriesMap.insert("Monaco", "MCO");
	m_EuropeCountriesMap.insert("Serbia and Montenegro", "SCG");
	m_EuropeCountriesMap.insert("Netherlands", "NDL");
	m_EuropeCountriesMap.insert("Norway", "NOR");
	m_EuropeCountriesMap.insert("Poland", "POL");
	m_EuropeCountriesMap.insert("Portugal", "PRT");
	m_EuropeCountriesMap.insert("Romania", "ROU");
	m_EuropeCountriesMap.insert("Russia", "RUS");
	m_EuropeCountriesMap.insert("San Marino", "SMR");
	m_EuropeCountriesMap.insert("Slovakia", "SVK");
	m_EuropeCountriesMap.insert("Slovenia", "SVN");
	m_EuropeCountriesMap.insert("Spain", "ESP");
	m_EuropeCountriesMap.insert("Svalbard and Jan Mayen Islands", "SJM");
	m_EuropeCountriesMap.insert("Sweden", "SWE");
	m_EuropeCountriesMap.insert("Switzerland", "CHE");
	m_EuropeCountriesMap.insert("Turkey", "TUR");
	m_EuropeCountriesMap.insert("Ukraine", "UKR");
	m_EuropeCountriesMap.insert("United Kingdom", "GBR");
	m_EuropeCountriesMap.insert("Vatican City State", "VAT");

	m_MiddleEastCountriesMap.insert("Bahrain",   "BHR");
	m_MiddleEastCountriesMap.insert("Iraq",      "IRQ");
	m_MiddleEastCountriesMap.insert("Iran",      "IRN");
	m_MiddleEastCountriesMap.insert("Israel",    "ISR");
	m_MiddleEastCountriesMap.insert("Jordan",    "JOR");
	m_MiddleEastCountriesMap.insert("Kuwait",    "KWT");
	m_MiddleEastCountriesMap.insert("Lebanon",   "LBN");
	m_MiddleEastCountriesMap.insert("Oman",      "OMN");
	m_MiddleEastCountriesMap.insert("Palestine", "PSE");
	m_MiddleEastCountriesMap.insert("Qatar",     "QAT");
	m_MiddleEastCountriesMap.insert("Saudi Arabia", "SAU");
	m_MiddleEastCountriesMap.insert("Syria",     "SYR");
	m_MiddleEastCountriesMap.insert("United Arab Emirates", "ARE");
	m_MiddleEastCountriesMap.insert("Yemen",     "YEM");

	m_OceaniaCountriesMap.insert("Australia", "AUS");
	m_OceaniaCountriesMap.insert("Fiji", "FJI");
	m_OceaniaCountriesMap.insert("French Polynesia", "PYF");
	m_OceaniaCountriesMap.insert("Guam", "GUM");
	m_OceaniaCountriesMap.insert("Kiribati", "KIR");
	m_OceaniaCountriesMap.insert("Marshall Islands", "MHL");
	m_OceaniaCountriesMap.insert("Micronesia", "FSM");
	m_OceaniaCountriesMap.insert("New Caledonia", "NCL");
	m_OceaniaCountriesMap.insert("New Zealand", "NZL");
	m_OceaniaCountriesMap.insert("Papua New Guinea", "PNG");
	m_OceaniaCountriesMap.insert("Samoa", "WSM");
	m_OceaniaCountriesMap.insert("Samoa, American", "ASM");
	m_OceaniaCountriesMap.insert("Solomon Islands", "SLB");
	m_OceaniaCountriesMap.insert("Tonga", "TON");
	m_OceaniaCountriesMap.insert("Vanuatu", "VUT");

	m_CaribbeanCountriesMap.insert("Anguilla", "AIA");
	m_CaribbeanCountriesMap.insert("Antigua and Barbuda", "ATG");
	m_CaribbeanCountriesMap.insert("Aruba", "ABW");
	m_CaribbeanCountriesMap.insert("Bahamas", "BHS");
	m_CaribbeanCountriesMap.insert("Barbados", "BRB");
//	m_CaribbeanCountriesMap.insert("Bonaire, Saint Eustatius and Saba", "");
	m_CaribbeanCountriesMap.insert("Cayman Islands", "CYM");
	m_CaribbeanCountriesMap.insert("Cuba", "CUB");
//	m_CaribbeanCountriesMap.insert("Curaçao", "");
	m_CaribbeanCountriesMap.insert("Dominica", "DMA");
	m_CaribbeanCountriesMap.insert("Dominican Republic", "DOM");
	m_CaribbeanCountriesMap.insert("Grenada", "GRD");
	m_CaribbeanCountriesMap.insert("Guadaloupe", "GLP");
	m_CaribbeanCountriesMap.insert("Haiti", "HTI");
	m_CaribbeanCountriesMap.insert("Jamaica", "JAM");
	m_CaribbeanCountriesMap.insert("Martinique", "MTQ");
	m_CaribbeanCountriesMap.insert("Montserrat", "MSR");
	m_CaribbeanCountriesMap.insert("Puerto Rico", "PRI");
//	m_CaribbeanCountriesMap.insert("Saint-Barthélemy", "");
	m_CaribbeanCountriesMap.insert("St. Kitts and Nevis", "KNA");
	m_CaribbeanCountriesMap.insert("St. Lucia", "LCA");
//	m_CaribbeanCountriesMap.insert("Saint Martin", "");
	m_CaribbeanCountriesMap.insert("St. Vincent and the Grenadines", "VCT");
//	m_CaribbeanCountriesMap.insert("Sint Maarten", "");
	m_CaribbeanCountriesMap.insert("Trinidad and Tobago",      "TTO");
	m_CaribbeanCountriesMap.insert("Turks and Caicos Islands", "TCA");
	m_CaribbeanCountriesMap.insert("Virgin Islands (British)", "VGB");
	m_CaribbeanCountriesMap.insert("Virgin Islands (US)",      "VIR");

}

IsoCountries::~IsoCountries()
{
	m_instance = 0;
}

IsoCountries *IsoCountries::instance()
{
	if (m_instance == 0) {
		m_instance = new IsoCountries();
	}
	return m_instance;
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
	case IsoCountries::Europe:         return m_EuropeCountriesMap.keys();
	case IsoCountries::MiddleEast:     return m_MiddleEastCountriesMap.keys();
	case IsoCountries::NorthAmerica:   return m_NorthAmericaCountriesMap.keys();
	case IsoCountries::Oceania:        return m_OceaniaCountriesMap.keys();
	case IsoCountries::SouthAmerica:   return m_SouthAmericaCountriesMap.keys();
	case IsoCountries::TheCaribbean:   return m_CaribbeanCountriesMap.keys();
	default:
		return m_EuropeCountriesMap.keys();
	}
}

QString IsoCountries::counrtyCode(const QString &countryName) const
{
	return QString();
}

QStringList IsoCountries::countryCodes(IsoCountries::WorldRegion regionCode) const
{
	switch (regionCode) {
	case IsoCountries::Africa:         return m_AfricaCountriesMap.values();
	case IsoCountries::Asia:           return m_AsiaCountriesMap.values();
	case IsoCountries::CentralAmerica: return m_CentralAmericaCountriesMap.values();
	case IsoCountries::Europe:         return m_EuropeCountriesMap.values();
	case IsoCountries::MiddleEast:     return m_MiddleEastCountriesMap.values();
	case IsoCountries::NorthAmerica:   return m_NorthAmericaCountriesMap.values();
	case IsoCountries::Oceania:        return m_OceaniaCountriesMap.values();
	case IsoCountries::SouthAmerica:   return m_SouthAmericaCountriesMap.values();
	case IsoCountries::TheCaribbean:   return m_CaribbeanCountriesMap.values();
	default:
		return m_EuropeCountriesMap.values();
	}
}
