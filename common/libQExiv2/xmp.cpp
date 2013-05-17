/*
 * file: xmp.cpp
 *
 * QExiv2 XMP methods
 *
 */

#include <QDebug>

#include "QExiv2.h"
#include "QExiv2_p.h"

bool QExiv2::hasXmp() const
{
	return !d->xmpMetadata.empty();
}

bool QExiv2::isXmpWritable() const
{
	return d->isMetadataWritable(Exiv2::mdXmp);
}

QList<exifData> QExiv2::xmpDataList() const
{
	if (d->xmpMetadata.empty()) {
		QList<struct exifData>();
	}

	QList<struct exifData> lst;

	// fill list;
	Exiv2::XmpData::const_iterator end = d->xmpMetadata.end();
	for (Exiv2::XmpData::const_iterator md = d->xmpMetadata.begin(); md != end; ++md) {
		struct exifData ed;

		ed.key      = QString(md->key().c_str());
		ed.family   = QString(md->familyName());
		ed.group    = QString(md->groupName().c_str());
		ed.tagName  = QString(md->tagName().c_str());
		ed.tag      = md->tag();
		ed.typeName = QString(md->typeName());
		ed.typeId   = md->typeId();
		ed.count    = md->count();
		ed.value    = QString::fromUtf8(md->value().toString().c_str());
		lst.append(ed);
	}

	return lst;
}

bool QExiv2::removeXmpTag(const char *xmpTagName)
{
	try {
		Exiv2::XmpKey xmpKey(xmpTagName);
		Exiv2::XmpData::iterator it = d->xmpMetadata.findKey(xmpKey);
		if (it != d->xmpMetadata.end()) {
			qDebug() << __PRETTY_FUNCTION__ << "Erase key:" << xmpTagName;
			d->xmpMetadata.erase(it);
			return true;
		}
	} catch (Exiv2::Error &e) {
		d->printExiv2ExceptionError(QString("Cannot remove Xmp tag '%1' using Exiv2").arg(xmpTagName), e);
	}

	return false;
}

// Serialize the XMP data and output the XML XMP packet
QByteArray QExiv2::xmpPacket() const
{
	try {
		std::string xmpPacket;
		Exiv2::XmpData xmpData(d->xmpMetadata);

		if (0 != Exiv2::XmpParser::encode(xmpPacket, xmpData)) {
			//qDebug() << "Failed to serialize XMP data";
			throw Exiv2::Error(1, "Failed to serialize XMP data");
		} else {
			QByteArray ba(xmpPacket.c_str(), xmpPacket.size());
			//qDebug() << QString(xmpPacket.c_str());
			qDebug() << ba;
			return ba;
		}

		// Cleanup
		Exiv2::XmpParser::terminate();

	} catch (Exiv2::Error &e) {
		d->printExiv2ExceptionError("QExiv2::xmpPacket()", e);
	}

	return QByteArray();
}

QString QExiv2::xmpTagString(const char *xmpTagName, bool escapeCR) const
{
	try {
		Exiv2::XmpData xmpData(d->xmpMetadata);
		Exiv2::XmpKey key(xmpTagName);
		Exiv2::XmpData::const_iterator it = xmpData.findKey(key);
		if (it != xmpData.end()) {
			std::ostringstream os;
			os << *it;
			QString tagValue = QString::fromUtf8(os.str().c_str());

			if (escapeCR)
				tagValue.replace('\n', ' ');

			return tagValue;
		}
	} catch (Exiv2::Error &e) {
		d->printExiv2ExceptionError(QString("Cannot find Xmp key '%1' into image using Exiv2").arg(xmpTagName), e);
	}

	return QString();
}

bool QExiv2::setXmpTagString(const char *xmpTagName, const QString& value)
{
	try {
		const std::string &txt(value.toUtf8().constData());
		Exiv2::Value::AutoPtr xmpTxtVal = Exiv2::Value::create(Exiv2::xmpText);
		xmpTxtVal->read(txt);
		d->xmpMetadata[xmpTagName].setValue(xmpTxtVal.get());
		return true;
	} catch (Exiv2::Error &e) {
		d->printExiv2ExceptionError(QString("Cannot set Xmp tag string '%1' into image using Exiv2 ").arg(xmpTagName), e);
	}

	return false;
}

//****************************************************************************
// XMP Bag
//****************************************************************************
QStringList QExiv2::xmpTagStringBag(const char *xmpTagName, bool escapeCR) const
{
	try {
		Exiv2::XmpData xmpData(d->xmpMetadata);
		Exiv2::XmpKey key(xmpTagName);
		Exiv2::XmpData::const_iterator it = xmpData.findKey(key);
		if (it != xmpData.end()) {
			//qDebug() << __func__ << "TypeID: 0x" << QString::number(it->typeId(), 16);
			if (it->typeId() == Exiv2::xmpBag) {
				QStringList bag;
				for (int i = 0; i < it->count(); i++) {
					std::ostringstream os;
					os << it->toString(i);
					QString bagValue = QString::fromUtf8(os.str().c_str());
					if (escapeCR) {
						bagValue.replace('\n', ' ');
					}
					bag.append(bagValue);
				}
				return bag;
			}
		}
	} catch (Exiv2::Error &e) {
		d->printExiv2ExceptionError(QString("Cannot find Xmp key '%1' into image using Exiv2 ").arg(xmpTagName), e);
	}

	return QStringList();
}

bool QExiv2::setXmpTagStringBag(const char *xmpTagName, const QStringList &bag)
{
	try {
		if (bag.isEmpty()) {
			removeXmpTag(xmpTagName);
		} else {
			QStringList list = bag;
			Exiv2::Value::AutoPtr xmpTxtBag = Exiv2::Value::create(Exiv2::xmpBag);

			for (QStringList::const_iterator it = list.constBegin(); it != list.constEnd(); ++it) {
				const std::string &txt((*it).toUtf8().constData());
				xmpTxtBag->read(txt);
			}
			d->xmpMetadata[xmpTagName].setValue(xmpTxtBag.get());
		}
		return true;
	} catch (Exiv2::Error &e) {
		d->printExiv2ExceptionError(QString("Cannot set Xmp tag string Bag '%1' into image using Exiv2 ").arg(xmpTagName), e);
	}

	return false;
}

bool QExiv2::setXmpTagBag(const char *xmpTagName)
{
	try {
		Exiv2::Value::AutoPtr xmpTxtBag = Exiv2::Value::create(Exiv2::xmpBag);
		d->xmpMetadata[xmpTagName].setValue(xmpTxtBag.get());
		return true;
	} catch (Exiv2::Error &e) {
		d->printExiv2ExceptionError(QString("Cannot set Xmp tag Bag '%1' into image using Exiv2 ").arg(xmpTagName), e);
	}
	return false;
}

//****************************************************************************
// XMP Seq
//****************************************************************************
QStringList QExiv2::xmpTagStringSeq(const char *xmpTagName, bool escapeCR) const
{
	try {
		Exiv2::XmpData xmpData(d->xmpMetadata);
		Exiv2::XmpKey key(xmpTagName);
		Exiv2::XmpData::const_iterator it = xmpData.findKey(key);
		if (it != xmpData.end()) {
			if (it->typeId() == Exiv2::xmpSeq) {
				QStringList seq;
				for (int i = 0; i < it->count(); i++) {
					std::ostringstream os;
					os << it->toString(i);
					QString seqValue = QString::fromUtf8(os.str().c_str());

					if (escapeCR)
						seqValue.replace('\n', ' ');

					seq.append(seqValue);
				}
				qDebug() << "XMP String Seq (" << xmpTagName << "): " << seq;
				return seq;
			}
		}
	} catch (Exiv2::Error &e) {
		d->printExiv2ExceptionError(QString("Cannot find Xmp key '%1' into image using Exiv2 ").arg(xmpTagName), e);
	}

	return QStringList();
}

bool QExiv2::setXmpTagStringSeq(const char *xmpTagName, const QStringList &seq)
{
	try {
		if (seq.isEmpty()) {
			removeXmpTag(xmpTagName);
		} else {
			const QStringList list = seq;
			Exiv2::Value::AutoPtr xmpTxtSeq = Exiv2::Value::create(Exiv2::xmpSeq);

			for (QStringList::const_iterator it = list.constBegin(); it != list.constEnd(); ++it ) {
				const std::string &txt((*it).toUtf8().constData());
				xmpTxtSeq->read(txt);
			}
			d->xmpMetadata[xmpTagName].setValue(xmpTxtSeq.get());
		}
		return true;
	} catch (Exiv2::Error &e) {
		d->printExiv2ExceptionError(QString("Cannot set Xmp tag string Seq '%1' into image using Exiv2 ").arg(xmpTagName), e);
	}

	return false;
}

//****************************************************************************
// XMP Regions Tag
//****************************************************************************
bool QExiv2::hasXmpRegionTag() const
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

void QExiv2::xmpEraseRegionList()
{
	QList<XmpRegion> regions;
	QString s;

	removeXmpTag("Xmp.mwg-rs.Regions/mwg-rs:AppliedToDimensions/stDim:w");
	removeXmpTag("Xmp.mwg-rs.Regions/mwg-rs:AppliedToDimensions/stDim:h");
	removeXmpTag("Xmp.mwg-rs.Regions/mwg-rs:AppliedToDimensions/stDim:unit");
	removeXmpTag("Xmp.mwg-rs.Regions/mwg-rs:AppliedToDimensions");

	QString item = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]";
	QString name = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:Name";
	QString desc = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:Description";
	QString type = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:Type";

	QString focusUsage   = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:FocusUsage";
	QString barCodeValue = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:BarCodeValue";

	QString stArea  = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:Area";
	QString stAreaX = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:Area/stArea:x";
	QString stAreaY = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:Area/stArea:y";
	QString stAreaW = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:Area/stArea:w";
	QString stAreaH = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:Area/stArea:h";
	QString stAreaD = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:Area/stArea:d";
	QString stAreaUnit = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:Area/stArea:unit";

	int i = 1;
	while (1) {
		s = xmpTagString(item.arg(i).toLatin1(), false);
		if (s.isEmpty()) {
			// End of region list
			break;
		}

		removeXmpTag(stAreaX.arg(i).toLatin1());
		removeXmpTag(stAreaY.arg(i).toLatin1());
		removeXmpTag(stAreaD.arg(i).toLatin1());
		removeXmpTag(stAreaW.arg(i).toLatin1());
		removeXmpTag(stAreaH.arg(i).toLatin1());
		removeXmpTag(type.arg(i).toLatin1());
		removeXmpTag(name.arg(i).toLatin1());
		removeXmpTag(desc.arg(i).toLatin1());
		removeXmpTag(stAreaUnit.arg(i).toLatin1());
		removeXmpTag(stArea.arg(i).toLatin1());
		removeXmpTag(item.arg(i).toLatin1());

		i++;
	}

	removeXmpTag("Xmp.mwg-rs.Regions/mwg-rs:RegionList");
	removeXmpTag("Xmp.mwg-rs.Regions");
}

bool QExiv2::setXmpRegionList(const QList<XmpRegion> &regions)
{

	if (regions.isEmpty()) {
		//removeXmpTag("Xmp.mwg-rs");
		xmpEraseRegionList();
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

//****************************************************************************
// XMP LangAlt Tag
//****************************************************************************
QString QExiv2::xmpTagStringLangAlt(const char *xmpTagName, const QString &langAlt, bool escapeCR)
{
	try {
		Exiv2::XmpData xmpData(d->xmpMetadata);
		Exiv2::XmpKey key(xmpTagName);
	        for (Exiv2::XmpData::iterator it = xmpData.begin(); it != xmpData.end(); ++it) {
			if (it->key() == xmpTagName && it->typeId() == Exiv2::langAlt) {
				for (int i = 0; i < it->count(); i++) {
					std::ostringstream os;
					os << it->toString(i);
					QString lang;
					QString tagValue = QString::fromUtf8(os.str().c_str());
					//qDebug() << __func__ << tagValue;
					if (langAlt == lang) {
						if (escapeCR) {
							tagValue.replace('\n', ' ');
						}
						return tagValue;
					}
				}
			}
		}
	} catch (Exiv2::Error& e) {
		d->printExiv2ExceptionError(QString("Cannot find Xmp key '%1' into image using Exiv2").arg(xmpTagName), e);
	}

	return QString();
}

bool QExiv2::setXmpTagStringLangAlt(const char *xmpTagName, const QString &value, const QString &langAlt)
{
	try {
		QString language("x-default"); // default alternative language.

		if (!langAlt.isEmpty()) {
			language = langAlt;
		}

		QString txtLangAlt = QString("lang=%1 %2").arg(language).arg(value);

		const std::string &txt(txtLangAlt.toUtf8().constData());
		Exiv2::Value::AutoPtr xmpTxtVal = Exiv2::Value::create(Exiv2::langAlt);
#if 0
		// Search if an Xmp tag already exist.
		AltLangMap map = getXmpTagStringListLangAlt(xmpTagName, false);
		if (!map.isEmpty()) {
			for (AltLangMap::iterator it = map.begin(); it != map.end(); ++it) {
				if (it.key() != langAlt) {
					const std::string &val((*it).toUtf8().constData());
					xmpTxtVal->read(val);
			                kDebug() << *it;
				}
			}
		}
#endif
		xmpTxtVal->read(txt);
		removeXmpTag(xmpTagName);
		d->xmpMetadata.add(Exiv2::XmpKey(xmpTagName), xmpTxtVal.get());
		return true;
	} catch (Exiv2::Error &e) {
		d->printExiv2ExceptionError("Cannot set Xmp tag string lang-alt into image using Exiv2 ", e);
	}
	return false;
}
