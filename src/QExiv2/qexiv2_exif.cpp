//
// EXIF Functions
//

#include "QExiv2.h"
#include "qexiv2_p.h"

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
		Exiv2::ExifKey  key(tag);
		Exiv2::ExifData data(d->exifMetadata);
		Exiv2::ExifData::const_iterator it = data.findKey(key);
		if (it != data.end()) {
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
        Exiv2::ExifKey  key(tag);
        Exiv2::ExifData data(d->exifMetadata);
        Exiv2::ExifData::iterator it = data.findKey(key);
        if (it != data.end()) {
            char* const s = new char[(*it).size()];
            (*it).copy((Exiv2::byte*)s, Exiv2::bigEndian);
            QByteArray ba(s, (*it).size());
            delete[] s;
            return ba;
        }
    }
    catch (Exiv2::Error &e) {
        d->error(QString("%1 Cannot find Exif key '%2'").arg(__PRETTY_FUNCTION__).arg(tag), e);
    }
    return QByteArray();
}

#ifdef ENABLE_EXIF_PREVIEW
QImage QExiv2::previewImage() const
{
	if (!isValid()) {
		return QImage();
	}

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
		d->error(__PRETTY_FUNCTION__, e);
	}

	return QImage();
}
#endif
