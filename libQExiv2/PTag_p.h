#ifndef P_TAG_PRIVATE_H
#define P_TAG_PRIVATE_H

#include <QSharedData>
#include <QString>
#include <QRectF>

#include "MwgRegion.h"
#include "MPRegion.h"

class PTagDataPrivate : public QSharedData
{
	public:
		PTagDataPrivate() {}

	public:
		unsigned int	m_type;		// MP or MWG
		QString		m_text;		// Description
		QRectF		m_region;	// Normalized [0, 1] region rect

		QString m_mwgName;
		QString m_mwgDescription;
		QString m_mwgType;		// "Face" | "Pet" | "Focus" | "BarCode"
		QString m_mwgBarcodeValue;

		double m_mwgStAreaX;	// X coordinate of the center of the area (point, circle, rectangle)
		double m_mwgStAreaY;	// Y coordinate of the center of the area (point, circle, rectangle)
		double m_mwgStAreaW;	// Width of the area (rectangle)
		double m_mwgStAreaH;	// Height of the area (rectangle)
		double m_mwgStAreaD;	// Diameter of area (circle)
		QString m_mwgStAreaUnit; // Only "normalized"

		MwgRegionInfo m_mwgRegionInfo;
		MPRegionInfo  m_mpRegionInfo;
};

#endif
