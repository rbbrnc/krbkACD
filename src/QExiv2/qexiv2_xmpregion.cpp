/** QExiv2 XMP Regions Tag methods **/
#include <QDebug>

#include "QExiv2.h"
#include "qexiv2_p.h"

bool QExiv2::xmpHasRegionTags() const
{
	if (d->xmpMetadata.empty()) {
		return false;
	}

	if (!xmpTagString("Xmp.mwg-rs.Regions").isEmpty()) {
		return true;
	}

	if (!xmpTagString("Xmp.MP.RegionInfo").isEmpty()) {
		return true;
	}

	return false;
}

void QExiv2::xmpEraseRegions()
{
	//qDebug() << __PRETTY_FUNCTION__;
	try {
		Exiv2::XmpData &md = d->xmpMetadata;
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

void QExiv2::xmpSetMwgRegionList(const MwgRegionList &regions)
{
	xmpEraseRegions();
	if (regions.isEmpty()) {
		//xmpEraseRegions();
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
		xmpSetMwgRegion(regions.at(i), i + 1);
	}
}

// Set metadata mwg-rs regions
// Use write to save changes.
void QExiv2::xmpSetMwgRegion(const MwgRegion &region, int n)
{
	if (n <= 0) {
		qDebug() << __PRETTY_FUNCTION__ << "Cannot set index <= 0";
		return;
	}

	//qDebug() << __PRETTY_FUNCTION__ << region << n;

	QRectF stArea = region.stArea();

	setXmpTagString(MwgRs::regionListTag("Area/stArea:x", n), QString::number(stArea.x()));
	setXmpTagString(MwgRs::regionListTag("Area/stArea:y", n), QString::number(stArea.y()));

	switch (region.shape()) {
	case MwgRs::Point:
		break;
	case MwgRs::Circle:
		setXmpTagString(MwgRs::regionListTag("Area/stArea:d", n), QString::number(stArea.width()));
		break;
	case MwgRs::Rectangle:
	default:
		setXmpTagString(MwgRs::regionListTag("Area/stArea:w", n), QString::number(stArea.width()));
		setXmpTagString(MwgRs::regionListTag("Area/stArea:h", n), QString::number(stArea.height()));
		break;
	}

	setXmpTagString(MwgRs::regionListTag("Area/stArea:unit", n), "normalized");
	setXmpTagString(MwgRs::regionListTag("Name", n), region.name());
	setXmpTagString(MwgRs::regionListTag("Description", n), region.description());

	switch (region.type()) {
	case MwgRs::Pet:
		setXmpTagString(MwgRs::regionListTag("Type", n), "Pet");
		break;
	case MwgRs::Barcode:
		setXmpTagString(MwgRs::regionListTag("Type", n), "Barcode");
		break;
	case MwgRs::Focus:
		setXmpTagString(MwgRs::regionListTag("Type", n), "Focus");
		// XXX: todo set focus usage
		//setXmpTagString(MwgRs::regionListTag("FocusUsage", n), "Focus");
		break;
	case MwgRs::Face:
	default:
		setXmpTagString(MwgRs::regionListTag("Type", n), "Face");
		break;
	}
}

// Get mwg-rs regions from metadata
MwgRegionList QExiv2::xmpMwgRegionList() const
{
	if (d->xmpMetadata.empty()) {
		return MwgRegionList();
	}

	if (xmpTagString("Xmp.mwg-rs.Regions").isEmpty()) {
		return MwgRegionList();
	}

	QString s;
	s = xmpTagString("Xmp.mwg-rs.Regions/mwg-rs:AppliedToDimensions/stDim:w");
	if (s.isEmpty()) {
		qDebug() << __PRETTY_FUNCTION__
			 << "Invalid Regions -- stDim:w Missing";
		return MwgRegionList();
	}

	qreal dimW = s.toDouble();
	//qDebug() << "stDim:w" << s << ":" << dimW;

	s = xmpTagString("Xmp.mwg-rs.Regions/mwg-rs:AppliedToDimensions/stDim:h");
	if (s.isEmpty()) {
		qDebug() << __PRETTY_FUNCTION__
			 << "Invalid Regions -- stDim:h Missing";
		return MwgRegionList();
	}
	qreal dimH = s.toDouble();
	//qDebug() << "stDim:h" << s << ":" << dimH;

// XXX:
//	s = xmpTagString("Xmp.mwg-rs.Regions/mwg-rs:AppliedToDimensions/stDim:unit");
//	if (!s.isEmpty()) {
//	}

	MwgRegionList rl;
	int i = 1;
	while (1) {
		qreal x = 0.0;
		qreal y = 0.0;
		qreal w = 0.0;
		qreal h = 0.0;
		qreal d = 0.0;

		// get mwg-rs:RegionList[%1]
		if (xmpTagString(MwgRs::regionListTag("", i)).isEmpty()) {
			// End of region list
			break;
		}

		// stArea x,y,w,h,unit
		s = xmpTagString(MwgRs::regionListTag("Area/stArea:x", i));
		if (s.isEmpty()) {
			qDebug() << "Invalid region" << i << "-- stArea:x Missing";
			break;
		}
		x = s.toDouble();

		s = xmpTagString(MwgRs::regionListTag("Area/stArea:y", i));
		if (s.isEmpty()) {
			qDebug() << "Invalid region" << i << "-- stArea:y Missing";
			break;
		}
		y = s.toDouble();

		s = xmpTagString(MwgRs::regionListTag("Area/stArea:d", i));
		if (!s.isEmpty()) {
			d = s.toDouble();
		}

		s = xmpTagString(MwgRs::regionListTag("Area/stArea:w", i));
		if (!s.isEmpty()) {
			w = s.toDouble();
		}

		s = xmpTagString(MwgRs::regionListTag("Area/stArea:h", i));
		if (!s.isEmpty()) {
			h = s.toDouble();
		}

		if (d > 0) {
			w = d; // use w for diameter
		}

		MwgRegion r(QRectF(x,y,w,h), QSizeF(dimW,dimH), true);

		r.setName(xmpTagString(MwgRs::regionListTag("Name", i)));
		r.setDescription(xmpTagString(MwgRs::regionListTag("Description", i)));

		s = xmpTagString(MwgRs::regionListTag("Type", i));
		if (0 == QString::compare("Face", s, Qt::CaseInsensitive)) {
			r.setType(MwgRs::Face);
		} else if (0 == QString::compare("Focus", s, Qt::CaseInsensitive)) {
			s = xmpTagString(MwgRs::regionListTag("FocusUsage", i));
			// TODO parse focus usage
			r.setType(MwgRs::Focus);
		} else if(0 == QString::compare("Pet", s, Qt::CaseInsensitive)) {
			r.setType(MwgRs::Pet);
		} else if(0 == QString::compare("Barcode", s, Qt::CaseInsensitive)) {
			r.setType(MwgRs::Barcode);
			s = xmpTagString(MwgRs::regionListTag("BarCodeValue", i));
		} else {
		}

//		s = xmpTagString(MwgRs::regionListTag("Area/stArea:unit", i));
//		if (!s.isEmpty()) {
//		}

		// Add region to region list
		rl.append(r);

		i++;
	}

	return rl;
}
