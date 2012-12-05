#include <QStringList>

#include <QDebug>

#include "PTag.h"
#include "PTag_p.h"

PTag::PTag() : d(new PTagDataPrivate),
	m_type(PTag::MwgRegionType)
{

}

PTag::PTag(const PTag& other) : d(other.d),
	m_type(other.m_type)
{
}

PTag::~PTag()
{

}

PTag& PTag::operator=(const PTag& other)
{
	m_type = other.m_type;
	d = other.d;
	return *this;
}

enum PTag::RegionType PTag::type() const
{
	return m_type;
}

void PTag::setType(PTag::RegionType type)
{
	m_type = type;
}


QString PTag::description() const
{
	return (m_type == PTag::MwgRegionType) ?  d->m_mwgRegionInfo.description() : QString();
}

void PTag::setDescription(const QString &desc)
{
	if (m_type == PTag::MwgRegionType) {
		d->m_mwgRegionInfo.setDescription(desc);
	}
}

QString PTag::name() const
{
	return (m_type == PTag::MwgRegionType) ? d->m_mwgRegionInfo.name() : d->m_mpRegionInfo.name();
}

void PTag::setName(const QString &name)
{
	if (m_type == PTag::MwgRegionType) {
		d->m_mwgRegionInfo.setName(name);
	} else {
		d->m_mpRegionInfo.setName(name);
	}
}

QRectF PTag::region() const
{
	return d->m_region;
}

void PTag::setRegion(const QString &region)
{
	if (region.isEmpty()) {
		return;
	}

	QStringList sl = region.split(",");
	if (sl.isEmpty() || sl.size() != 4) {
		qDebug() << __PRETTY_FUNCTION__ << "wrong size";
		return;
	}
	d->m_region = QRectF(sl.at(0).toDouble(),
			     sl.at(1).toDouble(),
			     sl.at(2).toDouble(),
			     sl.at(3).toDouble());

//	qDebug() << __PRETTY_FUNCTION__ << "m_region:" << d->m_region;
}

void PTag::debug() const
{
	qDebug() << __PRETTY_FUNCTION__
		 << "Region Type:" << m_type
		 << "Name:" << name()
		 << "Description:" << description()
		 << "m_region:" << d->m_region;
}
