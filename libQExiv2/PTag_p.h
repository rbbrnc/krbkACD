#ifndef P_TAG_PRIVATE_H
#define P_TAG_PRIVATE_H

#include <QSharedData>
#include <QString>
#include <QRectF>

class PTagDataPrivate : public QSharedData
{
	public:
		PTagDataPrivate() {}

	public:
		unsigned int	m_type;
		QString		m_text;		// Description
		QRectF		m_region;	// Normalized [0, 1] region rect
};

#endif
