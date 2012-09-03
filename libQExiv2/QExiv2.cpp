#include <QFile>
#include <QFileInfo>
#include <QDebug>

#include "QExiv2.h"
#include "QExiv2_p.h"

QExiv2::QExiv2() : d(new QExiv2DataPrivate)
{
}

QExiv2::QExiv2(const QExiv2& other) : d(other.d)
{
}

QExiv2::QExiv2(const QString& filePath) : d(new QExiv2DataPrivate)
{
	load(filePath);
}

QExiv2::~QExiv2()
{
}

QExiv2& QExiv2::operator=(const QExiv2& other)
{
	d = other.d;
	return *this;
}

bool QExiv2::loadFromData(const QByteArray& data)
{
	if (data.isEmpty()) {
		qDebug() << __PRETTY_FUNCTION__ << "Empty data";
		return false;
	}

	try {
		Exiv2::Image::AutoPtr image;
		image = Exiv2::ImageFactory::open((Exiv2::byte *) data.constData(), data.size());
		image->readMetadata();

		d->exifMetadata = image->exifData();
		d->iptcMetadata = image->iptcData();
		d->xmpMetadata  = image->xmpData();
		d->imageComment = image->comment();

		//d->debug();
		d->image = image;
		return true;

	} catch (Exiv2::Error &e) {
		d->printExiv2ExceptionError("Cannot load metadata using Exiv2", e);
	}

	return false;
}

bool QExiv2::load(const QString& filePath)
{
	if (filePath.isEmpty()) {
		qDebug() << __PRETTY_FUNCTION__ << "Empty filePath";
		return false;
	}

	// Check if file exists and is readable!!!!
	QFileInfo info(filePath);
	if (!info.isReadable()) {
		qDebug() << "File '" << info.filePath().toAscii().constData() << "' is not readable.";
		return false;
	}

	try {
		Exiv2::Image::AutoPtr image;
		image = Exiv2::ImageFactory::open((const char *)(QFile::encodeName(filePath)));
		image->readMetadata();

		d->exifMetadata = image->exifData();
		d->iptcMetadata = image->iptcData();
		d->xmpMetadata  = image->xmpData();
		d->imageComment = image->comment();

		d->image = image;
		return true;

	} catch (Exiv2::Error &e) {
		d->printExiv2ExceptionError("Cannot load metadata using Exiv2", e);
	}

	return false;
}

// Se il file non ha i permessi in scrittura scatta l'exception
bool QExiv2::save()
{
	bool update = false;

	try {
		if (isImgCommentWritable()) {
			if (d->image->comment() != d->imageComment) {
				d->image->setComment(d->imageComment);
				update = true;
			} else {
				qDebug() << __PRETTY_FUNCTION__ << "Same Image Comment";
			}
		}

		if (isXmpWritable()) {
			//d->xmpMetadata.sortByKey();
			d->image->setXmpData(d->xmpMetadata);
			update = true;
		}

		if (update) {
			qDebug() << __PRETTY_FUNCTION__ << "write Metadata";
			d->image->writeMetadata();
		}

	       return true;

	} catch (Exiv2::Error& e) {
		d->printExiv2ExceptionError("Cannot save metadata using Exiv2", e);
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
// XMP Functions
//////////////////////////////////////////////////////////////////////////////////////////
bool QExiv2::hasXmp() const
{
	return !d->xmpMetadata.empty();
}

bool QExiv2::isXmpWritable() const
{
	try {
	        Exiv2::AccessMode mode = d->image->checkMode(Exiv2::mdXmp);
		return (mode == Exiv2::amWrite || mode == Exiv2::amReadWrite);
	} catch (Exiv2::Error& e) {
		d->printExiv2ExceptionError(QString("Cannot check XMP access mode using Exiv2"), e);
	}

	return false;
}

bool QExiv2::removeXmpTag(const char* xmpTagName)
{
	try {
		Exiv2::XmpKey xmpKey(xmpTagName);
		Exiv2::XmpData::iterator it = d->xmpMetadata.findKey(xmpKey);
		if (it != d->xmpMetadata.end()) {
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
		Exiv2::XmpData::iterator it = xmpData.findKey(key);
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

QStringList QExiv2::xmpTagStringBag(const char *xmpTagName, bool escapeCR) const
{
	try {
		Exiv2::XmpData xmpData(d->xmpMetadata);
		Exiv2::XmpKey key(xmpTagName);
		Exiv2::XmpData::iterator it = xmpData.findKey(key);
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

bool QExiv2::setXmpTagStringBag(const char* xmpTagName, const QStringList& bag)
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
	} catch( Exiv2::Error& e ) {
		d->printExiv2ExceptionError(QString("Cannot set Xmp tag string Bag '%1' into image using Exiv2 ").arg(xmpTagName), e);
	}

	return false;
}

//
QList<PTag> QExiv2::xmpMWG_RegionsTags() const
{
	QList<PTag> tl;

	if (d->xmpMetadata.empty()) {
		return tl;
	}

	if (xmpTagString("Xmp.mwg-rs.Regions", false).isEmpty()) {
		return tl;
	}

	QString mwgName = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:Name";
	QString mwgDescription = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:Description";
	QString mwgType = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:Type";

	QString mwgX = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:Area/stArea:x";
	QString mwgY = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:Area/stArea:y";
	QString mwgW = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:Area/stArea:w";
	QString mwgH = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:Area/stArea:h";
	QString mwgD = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:Area/stArea:d";

	QString mwgUnit = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:Area/stArea:unit";

	int i = 1;
	QString s;
	QString region;
	while (1) {
		PTag ptag;

		s = xmpTagString(mwgX.arg(i).toLatin1(), false);
		if (s.isEmpty()) {
			qDebug() << "stArea:x Missing";
			break;
		}
		//qDebug() << "stArea:x" << s;
		region = s + ",";

		s = xmpTagString(mwgY.arg(i).toLatin1(), false);
		if (s.isEmpty()) {
			qDebug() << "stArea:y Missing";
			break;
		}
		//qDebug() << "stArea:y" << s;
		region += s + ",";

		s = xmpTagString(mwgD.arg(i).toLatin1(), false);
		if (s.isEmpty()) {
			s = xmpTagString(mwgW.arg(i).toLatin1(), false);
			//qDebug() << "stArea:w" << s;
			region += s + ",";

			s = xmpTagString(mwgH.arg(i).toLatin1(), false);
			//qDebug() << "stArea:h" << s;
			region += s;
		} else {
			//qDebug() << "stArea:d" << s;
			region += s + "," + s;
		}
		ptag.setRegion(region);

		s = xmpTagString(mwgUnit.arg(i).toLatin1(), false);
		//qDebug() << "stArea:unit" << s;

		s = xmpTagString(mwgType.arg(i).toLatin1(), false);
		//qDebug() << "mwg-rs:Type" << s;

		s = xmpTagString(mwgDescription.arg(i).toLatin1(), false);
		ptag.setDescription(s);
		//qDebug() << "mwg-rs:Description" << s;

		s = xmpTagString(mwgName.arg(i).toLatin1(), false);
		//qDebug() << "mwg-rs:Name" << s;
		ptag.setName(s);

		tl.append(ptag);

		i++;
	}

	return tl;
}

QList<PTag> QExiv2::xmpPTags() const
{
	QList<PTag> tl;

	if (d->xmpMetadata.empty()) {
		return tl;
	}

	if (xmpTagString("Xmp.MP.RegionInfo", false).isEmpty()) {
		return tl;
	}

	QString textPath   = "Xmp.MP.RegionInfo/MPRI:Regions[%1]/MPReg:PersonDisplayName";
	QString regionPath = "Xmp.MP.RegionInfo/MPRI:Regions[%1]/MPReg:Rectangle";

	int i = 1;
	while (1) {
		QString region = xmpTagString(regionPath.arg(i).toLatin1(), false);
		if (region.isEmpty()) {
			break;
		}
		QString text = xmpTagString(textPath.arg(i).toLatin1(), false);

		PTag ptag;
		ptag.setType(PTag::MpRegionType);
		ptag.setRegion(region);
		ptag.setName(text);

		tl.append(ptag);
		i++;
	}

	return tl;
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

//////////////////////////////////////////////////////////////////////////////////////////
// Img Comment Functions
//////////////////////////////////////////////////////////////////////////////////////////
bool QExiv2::hasComment() const
{
	return !d->imageComment.empty();
}

QByteArray QExiv2::imgComment() const
{
	return QByteArray(d->imageComment.data(), d->imageComment.size());
}

bool QExiv2::setImgComment(const QByteArray& data)
{
	d->imageComment = std::string(data.data(), data.size());
	return true;
}

bool QExiv2::clearImgComment()
{
	return setImgComment(QByteArray());
}

bool QExiv2::isImgCommentWritable() const
{
	try {
		Exiv2::AccessMode mode = d->image->checkMode(Exiv2::mdComment);
		return (mode == Exiv2::amWrite || mode == Exiv2::amReadWrite);

	} catch (Exiv2::Error& e) {
		d->printExiv2ExceptionError(QString("Cannot check Comment access mode using Exiv2"), e);
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
// IPTC Functions
//////////////////////////////////////////////////////////////////////////////////////////
bool QExiv2::isIptcWritable() const
{
	try {
		Exiv2::AccessMode mode = d->image->checkMode(Exiv2::mdIptc);
		return (mode == Exiv2::amWrite || mode == Exiv2::amReadWrite);

	} catch (Exiv2::Error& e) {
		d->printExiv2ExceptionError(QString("Cannot check IPTC access mode using Exiv2"), e);
	}

	return false;
}

bool QExiv2::hasIptc() const
{
	return !d->iptcMetadata.empty();
}

bool QExiv2::clearIptc()
{
	try {
		d->iptcMetadata.clear();
		return true;

	} catch (Exiv2::Error& e) {
		d->printExiv2ExceptionError("Cannot clear Iptc data using Exiv2", e);
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
// EXIF Functions
//////////////////////////////////////////////////////////////////////////////////////////
bool QExiv2::isExifWritable() const
{
	try {
		Exiv2::AccessMode mode = d->image->checkMode(Exiv2::mdExif);
		return (mode == Exiv2::amWrite || mode == Exiv2::amReadWrite);

	} catch (Exiv2::Error& e) {
		d->printExiv2ExceptionError(QString("Cannot check EXIF access mode using Exiv2"), e);
	}

	return false;
}

bool QExiv2::hasExif() const
{
	return !d->exifMetadata.empty();
}

bool QExiv2::clearExif()
{
	try {
		d->exifMetadata.clear();
		return true;

	} catch (Exiv2::Error& e) {
		d->printExiv2ExceptionError("Cannot clear Exif data using Exiv2", e);
	}

	return false;
}


QList<exifData> QExiv2::exifDataList() const
{
	if (d->exifMetadata.empty()) {
		QList<struct exifData>();
	}

	QList<struct exifData> lst;

	// fill list;
	Exiv2::ExifData::const_iterator end = d->exifMetadata.end();
	for (Exiv2::ExifData::const_iterator md = d->exifMetadata.begin(); md != end; ++md) {
		struct exifData ed;

		ed.family   = QString(md->familyName());
		ed.group    = QString(md->groupName().c_str());
		ed.tagName  = QString(md->tagName().c_str());
		ed.key      = QString(md->key().c_str());
		ed.tag      = md->tag();
		ed.typeName = QString(md->typeName());
		ed.typeId   = md->typeId();
		ed.count    = md->count();
		ed.value    = QString(md->value().toString().c_str());
		lst.append(ed);
	}

	return lst;
}

QList<exifData> QExiv2::iptcDataList() const
{
	if (d->iptcMetadata.empty()) {
		QList<struct exifData>();
	}

	QList<struct exifData> lst;

	// fill list;
	Exiv2::IptcData::const_iterator end = d->iptcMetadata.end();
	for (Exiv2::IptcData::const_iterator md = d->iptcMetadata.begin(); md != end; ++md) {
		struct exifData ed;

		ed.key      = QString(md->key().c_str());
		ed.family   = QString(md->familyName());
		ed.group    = QString(md->groupName().c_str());
		ed.tagName  = QString(md->tagName().c_str());
		ed.tag      = md->tag();
		ed.typeName = QString(md->typeName());
		ed.typeId   = md->typeId();
		ed.count    = md->count();
		ed.value    = QString(md->value().toString().c_str());
		lst.append(ed);
	}

	return lst;
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
		ed.value    = QString(md->value().toString().c_str());
		lst.append(ed);
	}

	return lst;
}

QImage QExiv2::getPreviewImage() const
{
	try {
		// Get a list of preview images available in the image. The list is sorted
		// by the preview image pixel size, starting with the smallest preview.
		Exiv2::PreviewManager loader(*d->image);
		Exiv2::PreviewPropertiesList list = loader.getPreviewProperties();

//		qDebug() << __func__ << "# of preview:" << list.size();

		if (list.empty()) {
			return QImage();
		}

		// Some application logic to select one of the previews from the list
		Exiv2::PreviewPropertiesList::const_iterator pos = list.begin(); //selectPreview(list);

		// Get the selected preview image
		Exiv2::PreviewImage preview = loader.getPreviewImage(*pos);

		QImage image;
		QByteArray ba((const char*) preview.pData(), preview.size());
		if (!image.loadFromData(ba)) {
			return QImage();
		}

//		qDebug() << "[Preview] Type:" << QString(preview.extension().c_str())
//			 << "Size:" << preview.width() << "x" << preview.height();

		return image;

	} catch (Exiv2::Error& e) {
		d->printExiv2ExceptionError("Cannot get preview data using Exiv2", e);
	}

	return QImage();
}


QString QExiv2::detectLanguageAlt(const QString& value, QString& lang)
{
	// Ex. from an Xmp tag Xmp.tiff.copyright: "lang="x-default" (c) Gilles Caulier 2007"
	if (value.size() > 6 && value.startsWith(QString("lang=\""))) {
		int pos = value.indexOf(QString("\""), 6);
		if (pos != -1) {
			lang = value.mid(6, pos-6);
			return (value.mid(pos + 2));
		}
	}

	lang.clear();
	return value;
}

QString QExiv2::getXmpTagStringLangAlt(const char *xmpTagName, const QString &langAlt, bool escapeCR)
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
					tagValue = detectLanguageAlt(tagValue, lang);
					if (langAlt == lang) {
						if (escapeCR) {
							tagValue.replace('\n', ' ');
						}
						//qDebug() << tagValue;
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

// [EXIF] Get a string tag
QString QExiv2::getExifTagString(const char *exifTagName, bool escapeCR) const
{
	try {
		Exiv2::ExifKey exifKey(exifTagName);
		Exiv2::ExifData exifData(d->exifMetadata);
		Exiv2::ExifData::iterator it = exifData.findKey(exifKey);
		if (it != exifData.end()) {
#if 0
			// cfr KExiv2/libKexiv2: See B.K.O #184156 comment #13
			std::string val  = it->print(&exifData);
			QString tagValue = QString::fromLocal8Bit(val.c_str());
#else
			std::ostringstream os;
			os << *it;
			QString tagValue = QString::fromLocal8Bit(os.str().c_str());
#endif
			if (escapeCR) {
				tagValue.replace('\n', ' ');
			}

			return tagValue;
		}
	} catch (Exiv2::Error &e) {
		d->printExiv2ExceptionError(QString("Cannot find Exif key '%1' into image using Exiv2 ")
                                  .arg(exifTagName), e);
	}

	return QString();
}

// [EXIF] Get a DateTime Tag
QDateTime QExiv2::getExifTagDateTime(const char *exifTagName) const
{
	try {
		if (!d->exifMetadata.empty()) {
			Exiv2::ExifData exifData(d->exifMetadata);
			Exiv2::ExifKey key(exifTagName);

			qDebug() << "TAG Name:" << QString(key.tagName().c_str());
			qDebug() << "TAG Label:" << QString(key.tagLabel().c_str());
			qDebug() << "TAG GroupName:" << QString(key.groupName().c_str());
			qDebug() << "TAG Family Name:" << QString(key.familyName());
			qDebug() << "TAG Desc:" << QString(key.tagDesc().c_str());



			Exiv2::ExifData::iterator it = exifData.findKey(key);
			if (it != exifData.end()) {
				QDateTime dateTime = QDateTime::fromString(it->toString().c_str(), Qt::ISODate);
				if (dateTime.isValid()) {
					//qDebug() << __PRETTY_FUNCTION__ << "[EXIF]" << exifTagName << dateTime;
					return dateTime;
				}
			}
		}
	} catch (Exiv2::Error &e) {
		d->printExiv2ExceptionError(QString("Cannot find Exif key '%1' into image using Exiv2 ")
                                  .arg(exifTagName), e);
	}

	return QDateTime();
}

