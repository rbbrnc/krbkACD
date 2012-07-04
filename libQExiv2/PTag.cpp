#include <QStringList>

#include <QDebug>

#include "PTag.h"
#include "PTag_p.h"

PTag::PTag() : d(new PTagDataPrivate)
{
}

PTag::PTag(const PTag& other) : d(other.d)
{
}

PTag::~PTag()
{

}

PTag& PTag::operator=(const PTag& other)
{
	d = other.d;
	return *this;
}


QString PTag::text() const
{
	return d->m_text;
}

void PTag::setText(const QString &text)
{
	d->m_text = text;
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
		qDebug() << "PTag::" << __func__ << "wrong size";
		return;
	}
	d->m_region = QRectF(sl.at(0).toDouble(),
			     sl.at(1).toDouble(),
			     sl.at(2).toDouble(),
			     sl.at(3).toDouble());

//	qDebug() << "PTag::" << __func__ << "m_region:" << d->m_region;
}


void PTag::debug() const
{
	qDebug() << "PTag::" << "m_region:" << d->m_region << "m_text:" << d->m_text;
}
