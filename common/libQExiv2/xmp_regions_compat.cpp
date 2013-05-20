/*
 * file: xmp_regions.cpp
 *
 * QExiv2 XMP Regions Tag methods
 *
 */
#include <QDebug>

#include "QExiv2.h"
#include "QExiv2_p.h"

QList<XmpRegion> QExiv2::xmpRegionList() const
{
	QList<XmpRegion> regions;
	if (d->xmpMetadata.empty()) {
		return regions;
	}

	if (!(xmpTagString("Xmp.mwg-rs.Regions", false).isEmpty())) {
		regions = xmpMWGRegionList();
	}

	if (!(xmpTagString("Xmp.MP.RegionInfo", false).isEmpty())) {
		regions += xmpMPRegionList();
	}

	return regions;
}

// OLD XMP Regions
QList<XmpRegion> QExiv2::xmpMWGRegionList() const
{
	QList<XmpRegion> regions;
	QString s;

	s = xmpTagString("Xmp.mwg-rs.Regions/mwg-rs:AppliedToDimensions/stDim:w", false);
	if (s.isEmpty()) {
		qDebug() << "Invalid Xmp.mwg-rs.Regions -- stDim:w Missing";
	}
	qreal dimW = s.toDouble();
	qDebug() << "stDim:w" << s << ":" << dimW;

	s = xmpTagString("Xmp.mwg-rs.Regions/mwg-rs:AppliedToDimensions/stDim:h", false);
	if (s.isEmpty()) {
		qDebug() << "Invalid Xmp.mwg-rs.Regions -- stDim:h Missing";
	}
	qreal dimH = s.toDouble();
	qDebug() << "stDim:h" << s << ":" << dimH;

// XXX:
//	s = xmpTagString("Xmp.mwg-rs.Regions/mwg-rs:AppliedToDimensions/stDim:unit", false);
//	if (!s.isEmpty()) {
//	}

	QString item = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]";
	QString name = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:Name";
	QString desc = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:Description";
	QString type = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:Type";

//	QString focusUsage   = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:FocusUsage";
//	QString barCodeValue = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:BarCodeValue";

	QString stAreaX = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:Area/stArea:x";
	QString stAreaY = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:Area/stArea:y";
	QString stAreaW = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:Area/stArea:w";
	QString stAreaH = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:Area/stArea:h";
	QString stAreaD = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:Area/stArea:d";
	QString stAreaUnit = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:Area/stArea:unit";

	int i = 1;
	while (1) {

		XmpRegion r;
		r.setFormat(XmpRegion::MWG);

		qreal x = 0.0;
		qreal y = 0.0;
		qreal w = 0.0;
		qreal h = 0.0;
		qreal d = 0.0;

		s = xmpTagString(item.arg(i).toLatin1(), false);
		if (s.isEmpty()) {
			// End of region list
			break;
		}

		// stArea x,y,w,h,unit
		s = xmpTagString(stAreaX.arg(i).toLatin1(), false);
		if (s.isEmpty()) {
			qDebug() << "Invalid region" << i << "-- stArea:x Missing";
			break;
		}
		x = s.toDouble();

		s = xmpTagString(stAreaY.arg(i).toLatin1(), false);
		if (s.isEmpty()) {
			qDebug() << "Invalid region" << i << "-- stArea:y Missing";
			break;
		}
		y = s.toDouble();

		s = xmpTagString(stAreaD.arg(i).toLatin1(), false);
		if (!s.isEmpty()) {
			d = s.toDouble();
		}

		s = xmpTagString(stAreaW.arg(i).toLatin1(), false);
		if (!s.isEmpty()) {
			w = s.toDouble();
		}

		s = xmpTagString(stAreaH.arg(i).toLatin1(), false);
		if (!s.isEmpty()) {
			h = s.toDouble();
		}

		r.setSize(x, y, w, h, d, dimW, dimH);
		r.setType(xmpTagString(type.arg(i).toLatin1(), false));
		r.setName(xmpTagString(name.arg(i).toLatin1(), false));
		r.setDescription(xmpTagString(desc.arg(i).toLatin1(), false));
		r.setAreaUnit(xmpTagString(stAreaUnit.arg(i).toLatin1(), false));

		// Add region to region list
		regions.append(r);

		i++;
	}

	return regions;
}

bool QExiv2::setXmpRegionList(const QList<XmpRegion> &regions)
{
	if (regions.isEmpty()) {
		xmpEraseRegions();
		return true;
	}

	setXmpTagString("Xmp.mwg-rs.Regions/mwg-rs:AppliedToDimensions/stDim:w", "640");
	setXmpTagString("Xmp.mwg-rs.Regions/mwg-rs:AppliedToDimensions/stDim:h", "400");
	setXmpTagString("Xmp.mwg-rs.Regions/mwg-rs:AppliedToDimensions/stDim:unit", "pixel");

	setXmpTagBag("Xmp.mwg-rs.Regions/mwg-rs:RegionList");

	QString name = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:Name";
	QString desc = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:Description";
	QString type = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:Type";
//	QString focusUsage   = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:FocusUsage";
//	QString barCodeValue = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:BarCodeValue";
	QString stAreaX = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:Area/stArea:x";
	QString stAreaY = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:Area/stArea:y";
	QString stAreaW = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:Area/stArea:w";
	QString stAreaH = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:Area/stArea:h";
	QString stAreaD = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:Area/stArea:d";
	QString stAreaUnit = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:Area/stArea:unit";

	int j;
	for (int i = 0; i < regions.count(); i++) {
		j = i + 1;
		regions.at(i).debug();
		setXmpTagString(stAreaX.arg(j).toLatin1(), QString::number(regions.at(i).stAreaX()));
		setXmpTagString(stAreaY.arg(j).toLatin1(), QString::number(regions.at(i).stAreaY()));

		if (regions.at(i).shape() == XmpRegion::Rectangle) {
			setXmpTagString(stAreaW.arg(j).toLatin1(), QString::number(regions.at(i).stAreaW()));
			setXmpTagString(stAreaH.arg(j).toLatin1(), QString::number(regions.at(i).stAreaH()));
		} else if (regions.at(j).shape() == XmpRegion::Circle) {
			setXmpTagString(stAreaD.arg(j).toLatin1(), QString::number(regions.at(i).stAreaD()));
		}

		setXmpTagString(stAreaUnit.arg(j).toLatin1(), regions.at(i).areaUnit());
		setXmpTagString(name.arg(j).toLatin1(), regions.at(i).name());
		setXmpTagString(desc.arg(j).toLatin1(), regions.at(i).description());
		setXmpTagString(type.arg(j).toLatin1(), regions.at(i).type());
	}

	return true;
}
