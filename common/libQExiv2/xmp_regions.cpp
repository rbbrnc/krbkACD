/*
 * file: xmp_regions.cpp
 *
 * QExiv2 XMP Regions Tag methods
 *
 */
#include <QDebug>

#include "QExiv2.h"
#include "QExiv2_p.h"

bool QExiv2::xmpHasRegionTags() const
{
	if (d->xmpMetadata.empty()) {
		return false;
	}

	if (!xmpTagString("Xmp.mwg-rs.Regions", false).isEmpty()) {
		return true;
	}

	if (!xmpTagString("Xmp.MP.RegionInfo", false).isEmpty()) {
		return true;
	}

	return false;
}

void QExiv2::xmpEraseRegions()
{
	//qDebug() << __PRETTY_FUNCTION__;
	try {
		Exiv2::XmpData &md = d->xmpMetadata; // d->image->xmpData();
		Exiv2::XmpData::iterator it = md.begin();
		while (it != md.end()) {
			if (it->key().compare(0, 18, "Xmp.mwg-rs.Regions") == 0) {
				//qDebug() << "[ERASE]" << it->key().c_str();
				it = md.erase(it);
			} else {
				++it;
			}
		}
	} catch (Exiv2::Error& e) {
		d->error(QString("Cannot Erase Xmp tag '%1'using Exiv2 ").arg(__func__), e);
	}
}

void QExiv2::xmpSetMwgRegion(const MwgRegion &region, int n)
{
	if (n <=0) {
		qDebug() << __PRETTY_FUNCTION__ << "Cannot set index <= 0";
		return;
	}

	qDebug() << __PRETTY_FUNCTION__ << region << n;
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

	QRectF stArea = region.stArea();

	setXmpTagString(stAreaX.arg(n).toLatin1(), QString::number(stArea.x()));
	setXmpTagString(stAreaY.arg(n).toLatin1(), QString::number(stArea.y()));

	switch (region.shape()) {
	case MwgRegion::Point:
		break;
	case MwgRegion::Circle:
		setXmpTagString(stAreaD.arg(n).toLatin1(), QString::number(stArea.width()));
		break;
	case MwgRegion::Rectangle:
	default:
		setXmpTagString(stAreaW.arg(n).toLatin1(), QString::number(stArea.width()));
		setXmpTagString(stAreaH.arg(n).toLatin1(), QString::number(stArea.height()));
		break;
	}

	setXmpTagString(stAreaUnit.arg(n).toLatin1(), "normalized");
	setXmpTagString(name.arg(n).toLatin1(), region.name());
	setXmpTagString(desc.arg(n).toLatin1(), region.description());

	switch (region.type()) {
	case MwgRegion::Face:
		setXmpTagString(type.arg(n).toLatin1(), "Face");
		break;
	case MwgRegion::Pet:
		setXmpTagString(type.arg(n).toLatin1(), "Pet");
		break;
	case MwgRegion::Barcode:
		setXmpTagString(type.arg(n).toLatin1(), "Barcode");
		break;
	case MwgRegion::Focus:
	default:
		setXmpTagString(type.arg(n).toLatin1(), "Focus");
		break;
	}
}

void QExiv2::xmpSetMwgRegionList(const MwgRegionList &regions)
{
	if (regions.isEmpty()) {
		xmpEraseRegions();
		return;
	}

	// TODO:
	// Check region congruence to image dimensions and the applied dimensions
	setXmpTagString("Xmp.mwg-rs.Regions/mwg-rs:AppliedToDimensions/stDim:w",
			QString::number(d->image->pixelWidth()));

	setXmpTagString("Xmp.mwg-rs.Regions/mwg-rs:AppliedToDimensions/stDim:h",
			QString::number(d->image->pixelHeight()));

	setXmpTagString("Xmp.mwg-rs.Regions/mwg-rs:AppliedToDimensions/stDim:unit",
			"pixel");

	setXmpTagBag("Xmp.mwg-rs.Regions/mwg-rs:RegionList");
	for (int i = 0; i < regions.count(); i++) {
		xmpSetMwgRegion(regions.at(i), i+1);
	}
}

MwgRegionList QExiv2::xmpMwgRegionList() const
{
	if (d->xmpMetadata.empty()) {
		return MwgRegionList();
	}

	if (xmpTagString("Xmp.mwg-rs.Regions", false).isEmpty()) {
		return MwgRegionList();
	}

	QString s;
	s = xmpTagString("Xmp.mwg-rs.Regions/mwg-rs:AppliedToDimensions/stDim:w", false);
	if (s.isEmpty()) {
		qDebug() << __PRETTY_FUNCTION__
			 << "Invalid Xmp.mwg-rs.Regions -- stDim:w Missing";
		return MwgRegionList();
	}

	qreal dimW = s.toDouble();
	//qDebug() << "stDim:w" << s << ":" << dimW;

	s = xmpTagString("Xmp.mwg-rs.Regions/mwg-rs:AppliedToDimensions/stDim:h", false);
	if (s.isEmpty()) {
		qDebug() << __PRETTY_FUNCTION__
			 << "Invalid Xmp.mwg-rs.Regions -- stDim:h Missing";
		return MwgRegionList();
	}
	qreal dimH = s.toDouble();
	//qDebug() << "stDim:h" << s << ":" << dimH;

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

	MwgRegionList rl;
	int i = 1;
	while (1) {
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

		MwgRegion r;

		r.setStDimensions(dimW, dimH, MwgRegion::Pixel);
		if (d > 0) {
			r.setStArea(x, y, d, 0.0, MwgRegion::Normalized);
		} else {
			r.setStArea(x, y, w, h, MwgRegion::Normalized);
		}

//		r.setType(xmpTagString(type.arg(i).toLatin1(), false));
		r.setName(xmpTagString(name.arg(i).toLatin1(), false));
		r.setDescription(xmpTagString(desc.arg(i).toLatin1(), false));
//		r.setAreaUnit(xmpTagString(stAreaUnit.arg(i).toLatin1(), false));

		// Add region to region list
		rl.append(r);

		i++;
	}

	return rl;
}
