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
		image->readMetadata();

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
