#include <QDebug>

#include "QExiv2.h"
#include "QExiv2_p.h"

//////////////////////////////////////////////////////////////////////////////////////////
// EXIF Functions
//////////////////////////////////////////////////////////////////////////////////////////
bool QExiv2::isExifWritable() const
{
	return d->isMetadataWritable(Exiv2::mdExif);
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

// return a list of all Exif tags present.
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
		ed.value    = QString::fromUtf8(md->value().toString().c_str());
		lst.append(ed);
	}

	return lst;
}

// [EXIF] Get a string tag
QString QExiv2::exifTagString(const char *exifTagName, bool escapeCR) const
{
	try {
		Exiv2::ExifKey exifKey(exifTagName);
		Exiv2::ExifData exifData(d->exifMetadata);
		Exiv2::ExifData::const_iterator it = exifData.findKey(exifKey);
		if (it != exifData.end()) {
			std::ostringstream os;
			os << *it;
			QString tagValue = QString::fromLocal8Bit(os.str().c_str());
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
QDateTime QExiv2::exifTagDateTime(const char *exifTagName) const
{
	try {
		if (!d->exifMetadata.empty()) {
			Exiv2::ExifData exifData(d->exifMetadata);
			Exiv2::ExifKey key(exifTagName);
#if 0
			qDebug() << "TAG Name:" << QString(key.tagName().c_str());
			qDebug() << "TAG Label:" << QString(key.tagLabel().c_str());
			qDebug() << "TAG GroupName:" << QString(key.groupName().c_str());
			qDebug() << "TAG Family Name:" << QString(key.familyName());
			qDebug() << "TAG Desc:" << QString(key.tagDesc().c_str());
#endif

			Exiv2::ExifData::const_iterator it = exifData.findKey(key);
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
