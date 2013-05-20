/*
 * file: mp_regions.cpp
 *
 * QExiv2 XMP MP Regions Tag methods
 *
 */
#include <QDebug>

#include "QExiv2.h"
#include "QExiv2_p.h"

QList<XmpRegion> QExiv2::xmpMPRegionList() const
{
	QList<XmpRegion> regions;

	QString name = "Xmp.MP.RegionInfo/MPRI:Regions[%1]/MPReg:PersonDisplayName";
	QString rect = "Xmp.MP.RegionInfo/MPRI:Regions[%1]/MPReg:Rectangle";
	QString email = "Xmp.MP.RegionInfo/MPRI:Regions[%1]/MPReg:PersonEmailDigest";
	QString liveCID = "Xmp.MP.RegionInfo/MPRI:Regions[%1]/MPReg:PersonLiveCID";

	QString s;

	int i = 1;
	while (1) {
		s = xmpTagString(rect.arg(i).toLatin1(), false);
		if (s.isEmpty()) {
			break;
		}

		QStringList sl = s.split(",");
		if (sl.isEmpty() || sl.size() != 4) {
			qDebug() << __PRETTY_FUNCTION__ << "wrong size";
			continue;
		}

		qreal x = sl.at(0).toDouble();
		qreal y = sl.at(1).toDouble();
		qreal w = sl.at(2).toDouble();
		qreal h	= sl.at(3).toDouble();

		qreal dimW = d->image->pixelWidth();
		qreal dimH = d->image->pixelHeight();

		XmpRegion r;
		r.setFormat(XmpRegion::MP);
		r.setSize(x, y, w, h, 0, dimW, dimH);
		r.setName(xmpTagString(name.arg(i).toLatin1(), false));
		r.setPersonEmailDigest(xmpTagString(email.arg(i).toLatin1(), false));
		r.setPersonLiveCID(xmpTagString(liveCID.arg(i).toLatin1(), false));

		regions.append(r);
		i++;
	}

	return regions;
}
