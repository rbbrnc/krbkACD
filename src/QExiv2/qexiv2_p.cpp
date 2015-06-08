#include <QFile>
#include <QDebug>

#include "qexiv2_p.h"

QExiv2DataPrivate::QExiv2DataPrivate()
{
	// Register Iptc4xmpExt prefix for IptcExt group
	Exiv2::XmpProperties::registerNs("Iptc4xmpExt/", "Iptc4xmpExt");
	metadataValid = false;
}

QExiv2DataPrivate::~QExiv2DataPrivate()
{
    Exiv2::XmpProperties::unregisterNs();
}

bool QExiv2DataPrivate::readMetadata(const QByteArray &data)
{
	metadataValid = false;
	try {
		image = Exiv2::ImageFactory::open((Exiv2::byte *) data.constData(), data.size());
	} catch (Exiv2::Error &e) {
        error(Q_FUNC_INFO, e);
		return false;
	}

	return readMetadata();
}

bool QExiv2DataPrivate::readMetadata(const QString &filePath)
{
	metadataValid = false;

	try {
		image = Exiv2::ImageFactory::open((const char *)(QFile::encodeName(filePath)));
	} catch (Exiv2::Error &e) {
        error(Q_FUNC_INFO, e);
		return false;
	}

	return readMetadata();
}

bool QExiv2DataPrivate::readMetadata()
{
	metadataValid = false;

	if (image.get()) {
		try {
			image->readMetadata();
		} catch (Exiv2::Error &e) {
            error(Q_FUNC_INFO, e);
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
    qWarning() << QString("%1 (Error #%2:%3)").arg(msg.toLatin1().constData()).arg(e.code()).arg(e.what());
}

// Exiv2::MetadataId {mdNone = 0, mdExif = 1, mdIptc = 2, mdComment = 4, mdXmp = 8}
bool QExiv2DataPrivate::isMetadataWritable(Exiv2::MetadataId mid) const
{
	try {
		Exiv2::AccessMode mode = image->checkMode(mid);
		return (mode == Exiv2::amWrite || mode == Exiv2::amReadWrite);
    } catch (Exiv2::Error &e) {
        QString msg = "Cannot check ";
		switch (mid) {
        case Exiv2::mdNone:    msg += "mdNone";    break;
        case Exiv2::mdExif:    msg += "mdExif";    break;
        case Exiv2::mdIptc:    msg += "mdIptc";    break;
        case Exiv2::mdComment: msg += "mdComment"; break;
        case Exiv2::mdXmp:     msg += "mdXmp";     break;
		default:
            msg += "unknown";
            break;
		}
        msg += " access mode using Exiv2";
		error(msg, e);
	}
	return false;
}

QString QExiv2DataPrivate::errorString(Exiv2::Error &e) const
{
    return QString("[Exiv2 Error #%1:%2]").arg(e.code()).arg(e.what());
}
