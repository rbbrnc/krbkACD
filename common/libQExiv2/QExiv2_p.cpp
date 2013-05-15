#include <QDebug>

#include "QExiv2_p.h"

QExiv2DataPrivate::QExiv2DataPrivate()
{
	metadataValid = false;
}

bool QExiv2DataPrivate::readMetadata()
{
	metadataValid = false;

	if (image.get()) {
		try {
			image->readMetadata();
		} catch (Exiv2::Error &e) {
			printExiv2ExceptionError("Cannot read metadata using Exiv2", e);
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
	qDebug() << msg.toLatin1().constData()
		 << "(Error #" << e.code() << ":" << s.c_str() << ")";
}