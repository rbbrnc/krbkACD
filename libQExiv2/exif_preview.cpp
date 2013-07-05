#include <QDebug>

#include "QExiv2.h"
#include "QExiv2_p.h"

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

