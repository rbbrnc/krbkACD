#include <QSize>
#include <QRect>

#include <QDebug>

#include "mwgRegionInfo.h"

MwgRegionInfo::MwgRegionInfo()
{
	m_appliedToDimensions.width  = 640;
	m_appliedToDimensions.height = 480;
	m_appliedToDimensions.unit   = "pixel";
}

void MwgRegionInfo::setDimensions()
{

}

void MwgRegionInfo::insertRegion(QSizeF atd, QRectF sel, const QString &name)
{
	struct mwgRegion region;

//	if (atd.width() != m_appliedToDimensions.width) {
//		qDebug() << "A:";
//	} else {
		region.area.x = sel.left() / atd.width();
		region.area.w = sel.width() / atd.width();
//	}

//	if (atd.height() != m_appliedToDimensions.height) {
//		qDebug() << "B:";
//	} else {
		region.area.y = sel.top() / atd.height();
		region.area.h = sel.height() / atd.height();
//	}


	region.area.d = 0;
	region.area.unit = "normalized";

	region.name = name;

	m_regionList.append(region);
}

void MwgRegionInfo::removeRegion()
{

}

void MwgRegionInfo::save()
{
	for (int i = 0; i< m_regionList.count(); i++) {
		qDebug() << "Area:"
			 << m_regionList.at(i).area.x
			 << m_regionList.at(i).area.y
			 << m_regionList.at(i).area.w
			 << m_regionList.at(i).area.h;
	}
}
