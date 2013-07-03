#include <QDebug>

#include "QExiv2_p.h"

QExiv2DataPrivate::QExiv2DataPrivate()
{
	// Register standard namespace for IptcExt
	Exiv2::XmpProperties::registerNs("Iptc4xmpExt/", "Iptc4xmpExt");
	metadataValid = false;
}

bool QExiv2DataPrivate::readMetadata()
{
	metadataValid = false;

	if (image.get()) {
		try {
			image->readMetadata();
		} catch (Exiv2::Error &e) {
			error(__PRETTY_FUNCTION__, e);
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

// Print exiv2 errors
void QExiv2DataPrivate::error(const QString &msg, Exiv2::Error &e) const
{
	std::string s(e.what());
	qWarning() << msg.toLatin1().constData()
		 << "(Error #" << e.code()
		 << ":" << s.c_str() << ")";
}


// Exiv2::MetadataId {mdNone = 0, mdExif = 1, mdIptc = 2, mdComment = 4, mdXmp = 8}
bool QExiv2DataPrivate::isMetadataWritable(Exiv2::MetadataId mid) const
{
	try {
		Exiv2::AccessMode mode = image->checkMode(mid);
		return (mode == Exiv2::amWrite || mode == Exiv2::amReadWrite);

	} catch (Exiv2::Error& e) {
		QString msg = "Cannot check";
		switch (mid) {
		case Exiv2::mdNone:
			msg += " mdNone ";
		case Exiv2::mdExif:
			msg += " mdExif";
		case Exiv2::mdIptc:
			msg += " mdIptc ";
		case Exiv2::mdComment:
			msg += " mdComment ";
		case Exiv2::mdXmp:
			msg += " mdXmp ";
		default:
			msg += " unknown ";
		}
		msg += "access mode using Exiv2";
		error(msg, e);
	}
	return false;
}

