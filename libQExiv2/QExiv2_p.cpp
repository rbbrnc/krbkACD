#include <QDebug>

#include "QExiv2_p.h"

QExiv2DataPrivate::QExiv2DataPrivate()
{
	metadataValid = false;
}

bool QExiv2DataPrivate::readMetadata()
{
	if (image.get() == 0) {
		metadataValid = false;
	} else {
		qDebug() << __PRETTY_FUNCTION__ << "A";

		try {
			image->readMetadata();
		} catch (Exiv2::Error &e) {
			printExiv2ExceptionError("Cannot read metadata using Exiv2", e);
			metadataValid = false;
			return false;
		}

		exifMetadata = image->exifData();
		iptcMetadata = image->iptcData();
		xmpMetadata  = image->xmpData();
		imageComment = image->comment();

		metadataValid = true;
	}

	return metadataValid;
}

void QExiv2DataPrivate::printExiv2ExceptionError(const QString &msg, Exiv2::Error &e) const
{
	std::string s(e.what());
	qDebug() << msg.toAscii().constData()
		 << "(Error #" << e.code() << ":" << s.c_str() << ")";
}
