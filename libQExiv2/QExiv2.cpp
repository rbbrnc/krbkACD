#include <QFile>
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
		qDebug() << "QExiv2::" <<__func__ << "Empty data";
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
		qDebug() << __func__ << "Empty filePath";
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

		//d->debug();
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
				qDebug() << __func__ << "Same Image Comment";
			}
		}

		if (update) {
			qDebug() << __func__ << "write Metadata";
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

QStringList QExiv2::xmpTagStringBag(const char* xmpTagName, bool escapeCR) const
{
	try {
		Exiv2::XmpData xmpData(d->xmpMetadata);
		Exiv2::XmpKey key(xmpTagName);
		Exiv2::XmpData::iterator it = xmpData.findKey(key);
		if (it != xmpData.end()) {
			qDebug() << __func__ << "TypeID: 0x" << QString::number(it->typeId(), 16);
			if (it->typeId() == Exiv2::xmpBag) {
				qDebug() << __func__ << "Found a BAG!";
				QStringList bag;
				for (int i = 0; i < it->count(); i++) {
					std::ostringstream os;
					os << it->toString(i);
					QString bagValue = QString::fromUtf8(os.str().c_str());
					if (escapeCR)
						bagValue.replace('\n', ' ');

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

//
#if 0
QList<PTag> QExiv2::xmpMWG_RegionsTags() const
{
	QList<PTag> tl;

	if (d->xmpMetadata.empty())
		return tl;

	QString regionInfo = xmpTagString("Xmp.mwg-rs.Regions", false);
	if (regionInfo.isEmpty()) {
		return tl;
	}

	QString textPath   = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:Name";
	QString regionPath = "Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/MPReg:Rectangle";

	Xmp.mwg-rs.Regions/mwg-rs:RegionList[1]/mwg-rs:Area/stArea:x XmpText     8  0.439417
	Xmp.mwg-rs.Regions/mwg-rs:RegionList[1]/mwg-rs:Area/stArea:y XmpText     4  0.45
	Xmp.mwg-rs.Regions/mwg-rs:RegionList[1]/mwg-rs:Area/stArea:w XmpText     8  0.154908
	Xmp.mwg-rs.Regions/mwg-rs:RegionList[1]/mwg-rs:Area/stArea:h XmpText     8  0.295122
	Xmp.mwg-rs.Regions/mwg-rs:RegionList[1]/mwg-rs:Area/stArea:unit XmpText    10  normalized

	int i = 1;
	while (1) {
		QString region = xmpTagString(regionPath.arg(i).toLatin1(), false);
		if (region.isEmpty()) {
			break;
		}
		QString text = xmpTagString(textPath.arg(i).toLatin1(), false);
		PTag ptag;
		ptag.setRegion(region);
		ptag.setText(text);
		tl.append(ptag);
		i++;
	}

	return tl;
}
#endif

QList<PTag> QExiv2::xmpPTags() const
{
	QList<PTag> tl;

	if (d->xmpMetadata.empty())
		return tl;

	QString regionInfo = xmpTagString("Xmp.MP.RegionInfo", false);
	if (regionInfo.isEmpty()) {
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
		ptag.setRegion(region);
		ptag.setText(text);
		tl.append(ptag);
		i++;
	}

	qDebug() << "Widget::" <<__func__ << "3";
	return tl;
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
		ed.tag      = md->tag();
		ed.typeName = QString(md->typeName());
		ed.typeId   = md->typeId();
		ed.count    = md->count();
		ed.value    = QString(md->value().toString().c_str());
		lst.append(ed);
	}

	return lst;
}
#if 0
	// Get a list of preview images available in the image. The list is sorted
	// by the preview image pixel size, starting with the smallest preview.
	Exiv2::PreviewManager loader(*image);
	Exiv2::PreviewPropertiesList list = loader.getPreviewProperties();

	// Some application logic to select one of the previews from the list
	Exiv2::PreviewPropertiesList::const_iterator pos = selectPreview(list);

	// Get the selected preview image
	Exiv2::PreviewImage preview = loader.getPreviewImage(*pos);

	// Write the preview image to a file
	std::string name = "preview" + preview.extension();
	std::cout << "Writing preview" << " "
		  << preview.width() << "x" << preview.height() << " "
		  << "to file" << " " << name << std::endl;
	preview.writeFile(name);
#endif

