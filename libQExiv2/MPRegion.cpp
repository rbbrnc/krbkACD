#include <QStringList>
#include <QDebug>

#include "MPRegion.h"

QString MPRegionInfo::name() const
{
	return m_name;
}

void MPRegionInfo::setName(const QString &name)
{
	m_name = name;
}

QRectF MPRegionInfo::rect() const
{
	return m_rect;
}

void MPRegionInfo::setRect(const QString &rect)
{
	if (rect.isEmpty()) {
		return;
	}

	QStringList sl = rect.split(",");
	if (sl.isEmpty() || sl.size() != 4) {
		qDebug() << __PRETTY_FUNCTION__ << "wrong size";
		return;
	}
	m_rect = QRectF(sl.at(0).toDouble(),
			     sl.at(1).toDouble(),
			     sl.at(2).toDouble(),
			     sl.at(3).toDouble());

//	qDebug() << __PRETTY_FUNCTION__ << "m_rect:" << m_rect;
}



