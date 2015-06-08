//
// EXIF Functions
//

#include "QExiv2.h"
#include "qexiv2_p.h"

QByteArray QExiv2::exifTagData(const char *tag) const
{
    try {
        const Exiv2::ExifData data(d->exifMetadata);
        Exiv2::ExifData::const_iterator it = data.findKey(Exiv2::ExifKey(tag));
        if (it != data.end()) {
            char* const s = new char[(*it).size()];
            (*it).copy((Exiv2::byte *) s, Exiv2::bigEndian);
            QByteArray ba(s, (*it).size());
            delete[] s;
            return ba;
        }
    }
    catch (Exiv2::Error &e) {
        d->error(QString("%1 Cannot find Exif key '%2'").arg(__func__).arg(tag), e);
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

//		qDebug() << Q_FUNC_INFO << "# of preview:" << list.size();

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
        d->error(Q_FUNC_INFO, e);
	}

	return QImage();
}
#endif
