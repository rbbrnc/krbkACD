//
// EXIF Functions
//

#include "QExiv2.h"
#include "QExiv2_p.h"

#ifdef ENABLE_EXIF_WRITE
bool QExiv2::isExifWritable() const
{
	return d->isMetadataWritable(Exiv2::mdExif);
}

bool QExiv2::clearExif()
{
	try {
		d->exifMetadata.clear();
		return true;

	} catch (Exiv2::Error& e) {
		d->error(__PRETTY_FUNCTION__, e);
	}

	return false;
}
#endif

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
QString QExiv2::exifTagString(const char *tag, bool escapeCR) const
{
	try {
		Exiv2::ExifKey exifKey(tag);
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
		d->error(QString("%1 Cannot find Exif key '%2'").arg(__PRETTY_FUNCTION__).arg(tag), e);
	}

	return QString();
}

QByteArray QExiv2::exifTagData(const char *tag) const
{
    try {
        Exiv2::ExifKey exifKey(tag);
        Exiv2::ExifData exifData(d->exifMetadata);
        Exiv2::ExifData::iterator it = exifData.findKey(exifKey);
        if (it != exifData.end()) {
            char* const s = new char[(*it).size()];
            (*it).copy((Exiv2::byte*)s, Exiv2::bigEndian);
            QByteArray data(s, (*it).size());
            delete[] s;
            return data;
        }
    }
    catch (Exiv2::Error &e) {
        d->error(QString("%1 Cannot find Exif key '%2'").arg(__PRETTY_FUNCTION__).arg(tag), e);
    }
    return QByteArray();
}

// [EXIF] Get a DateTime Tag
// Creation date of the intellectual content (e.g. when a photo was taken)
QDateTime QExiv2::exifDateTimeOriginal() const
{
    QDateTime dt = QDateTime::fromString(exifTagString("Exif.Photo.DateTimeOriginal"), "yyyy:MM:dd HH:mm:ss");
    if (!dt.isValid()) {
        dt = QDateTime::fromString(exifTagString("Exif.Image.DateTimeOriginal"), "yyyy:MM:dd HH:mm:ss");
    }
    return dt;
}

// [EXIF] Get a DateTime Tag
// Creation date of the digital representation (e.g. when an image was digitized)
QDateTime QExiv2::exifDateTimeDigitized() const
{
    return QDateTime::fromString(exifTagString("Exif.Photo.DateTimeDigitized"), "yyyy:MM:dd HH:mm:ss");
}

// [EXIF] Get a DateTime Tag
// Modification date of the digital image file (e.g. when a file was modified by the user)
QDateTime QExiv2::exifDateTime() const
{
    return QDateTime::fromString(exifTagString("Exif.Image.DateTime"), "yyyy:MM:dd HH:mm:ss");
}

