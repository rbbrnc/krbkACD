#include <QDebug>

#include "MwgRegion.h"

MwgArea::MwgArea() :
	m_type(MwgArea::Rectangle),
	m_x(0),
	m_y(0),
	m_w(0),
	m_h(0),
	m_d(0),
	m_unit("normalized")
{
}

MwgArea::MwgArea(const MwgArea &other)
{
	m_type = other.m_type;
	m_x    = other.m_x;
	m_y    = other.m_y;
	m_w    = other.m_w;
	m_h    = other.m_h;
	m_d    = other.m_d;
	m_unit = other.m_unit;
}

MwgArea::~MwgArea()
{

}

MwgArea& MwgArea::operator=(const MwgArea &other)
{
	m_type = other.m_type;
	m_x    = other.m_x;
	m_y    = other.m_y;
	m_w    = other.m_w;
	m_h    = other.m_h;
	m_d    = other.m_d;
	m_unit = other.m_unit;
	return *this;
}

enum MwgArea::Type MwgArea::type() const
{
	return m_type;
}

QRectF MwgArea::rect() const
{
	return QRectF(m_x, m_y, m_w, m_h);
}

QPointF MwgArea::point() const
{
	return QPointF(m_x, m_y);
}

///////////////////////////////////////////////////////////////

QString MwgRegionInfo::name() const
{
	return m_name;
}

void MwgRegionInfo::setName(const QString &name)
{
	m_name = name;
}

QString MwgRegionInfo::description() const
{
	return m_description;
}

void MwgRegionInfo::setDescription(const QString &description)
{
	m_description = description;
}

QString MwgRegionInfo::type() const
{
	return m_type;
}

void MwgRegionInfo::setType(const QString &type)
{
	m_type = type;
}

QString MwgRegionInfo::barcodeValue() const
{
	return m_barcodeValue;
}

void MwgRegionInfo::setBarcodeValue(const QString &value)
{
	m_barcodeValue = value;
}

MwgArea MwgRegionInfo::area() const
{
	return m_area;
}

void MwgRegionInfo::setArea(const MwgArea &area)
{
	m_area = area;
}
