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

		MwgRegionInfo m_mwgRegionInfo;
		MPRegionInfo  m_mpRegionInfo;
};

#endif
