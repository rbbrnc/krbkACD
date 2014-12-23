#include <QFile>
#include <QFileInfo>
#include <QDebug>

#include "QExiv2.h"
#include "qexiv2_p.h"

///////////////////////////////////////////////////////////////////////////////
/// class QExiv2
///////////////////////////////////////////////////////////////////////////////
QExiv2::QExiv2() : d(new QExiv2DataPrivate)
{
}

QExiv2::QExiv2(const QString &filePath) : d(new QExiv2DataPrivate)
{
	load(filePath);
}

QExiv2::~QExiv2()
{
}

bool QExiv2::isValid() const
{
	return d->metadataValid;
}

///////////////////////////////////////////////////////////////////////////////
/// Image Comment (e.g jpeg comment data)
///////////////////////////////////////////////////////////////////////////////
bool QExiv2::hasComment() const
{
    return !d->imageComment.empty();
}

bool QExiv2::isImgCommentWritable() const
{
    return d->isMetadataWritable(Exiv2::mdComment);
}

void QExiv2::clearImgComment()
{
    d->imageComment.clear();
}

void QExiv2::setImgComment(const QByteArray &data)
{
    d->imageComment = std::string(data.data(), data.size());
}

QByteArray QExiv2::imgComment() const
{
    return QByteArray(d->imageComment.data(), d->imageComment.size());
}

///////////////////////////////////////////////////////////////////////////////
/// Check Metadata presence
///////////////////////////////////////////////////////////////////////////////
bool QExiv2::hasExif()    const { return !d->exifMetadata.empty(); }
bool QExiv2::hasIptc()    const { return !d->iptcMetadata.empty(); }
bool QExiv2::hasXmp()     const { return !d->xmpMetadata.empty();  }

bool QExiv2::isExifWritable() const { return d->isMetadataWritable(Exiv2::mdExif); }
bool QExiv2::isIptcWritable() const { return d->isMetadataWritable(Exiv2::mdIptc); }
bool QExiv2::isXmpWritable()  const { return d->isMetadataWritable(Exiv2::mdXmp);  }

///////////////////////////////////////////////////////////////////////////////
/// Erase Metadata
///////////////////////////////////////////////////////////////////////////////
void QExiv2::clearExif() { d->exifMetadata.clear(); }
void QExiv2::clearIptc() { d->iptcMetadata.clear(); }
void QExiv2::clearXmp()  { d->xmpMetadata.clear();  }


///////////////////////////////////////////////////////////////////////////////
/// Load Functions
///////////////////////////////////////////////////////////////////////////////
#if 0
bool QExiv2::loadFromData(const QByteArray& data)
{
	if (data.isEmpty()) {
		return false;
	}
	return d->readMetadata(data);
}
#endif

bool QExiv2::load(const QString &filePath)
{
	if (filePath.isEmpty()) {
		return false;
	}

	// Check if file exists and is readable!!!!
	QFileInfo info(filePath);
	if (!info.isReadable()) {
		qDebug() << "File '" << info.filePath().toLatin1().constData() << "' is not readable.";
		return false;
	}
    // Don't read directories
    if (info.isDir()) {
        return false;
    }

	return d->readMetadata(filePath);
}

///////////////////////////////////////////////////////////////////////////////
/// Save Functions
///////////////////////////////////////////////////////////////////////////////
bool QExiv2::save()
{
	if (!isValid()) {
		return false;
	}

    // Se il file non ha i permessi in scrittura scatta l'exception
	try {
		bool update = false;
		if (isImgCommentWritable()) {
			if (d->image->comment() != d->imageComment) {
				d->image->setComment(d->imageComment);
				update = true;
			}
        }

		if (isXmpWritable()) {
			d->image->setXmpData(d->xmpMetadata);
			update = true;
        } else {
            qDebug() << __func__ << "XMP Not writable";
        }

		if (update) {
//			qDebug() << __PRETTY_FUNCTION__ << "write Metadata";
			d->image->writeMetadata();
		}

        return true;

    } catch (Exiv2::Error &e) {
        d->error(__func__, e);
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////////
/// Return a list of all EXIF, IPTC or XMP tags present.
///////////////////////////////////////////////////////////////////////////////
template<class T>
QList<exifData> metadataList(const T& metadata)
{
    QList<struct exifData> lst;
    const auto data = metadata;
    if (data.empty()) {
        return lst;
    }

    for (auto i = data.begin(); i != data.end(); ++i) {
        struct exifData ed;

        ed.key      = QString::fromStdString(i->key());
        ed.family   = QString::fromUtf8(i->familyName());
        ed.group    = QString::fromStdString(i->groupName());
        ed.tagName  = QString::fromStdString(i->tagName());
        ed.tag      = i->tag();
        ed.typeName = QString::fromUtf8(i->typeName());
        ed.typeId   = i->typeId();
        ed.count    = i->count();
        ed.value    = QString::fromStdString(i->value().toString());
        lst.append(ed);
    }
    return lst;
}

QList<exifData> QExiv2::xmpDataList()  const { return metadataList(d->xmpMetadata);  }
QList<exifData> QExiv2::iptcDataList() const { return metadataList(d->iptcMetadata); }
QList<exifData> QExiv2::exifDataList() const { return metadataList(d->exifMetadata); }

///////////////////////////////////////////////////////////////////////////////
/// Get Metadata string by tag
///////////////////////////////////////////////////////////////////////////////
template<class T, class U>
QString stringTag(const T& metadata, const U& key, bool escapeCR)
{
    const auto data = metadata;
    const auto i = data.findKey(key);
    if (i != data.end()) {
        QString tagValue = QString::fromStdString(i->toString());
        if (escapeCR) {
            tagValue.replace('\n', ' ');
        }
        return tagValue;
    }
    return QString();
}

QString QExiv2::iptcTagString(const char *tag, bool escapeCR) const
{
    return stringTag(d->iptcMetadata, Exiv2::IptcKey(tag), escapeCR);
}

QString QExiv2::exifTagString(const char *tag, bool escapeCR) const
{
    return stringTag(d->exifMetadata, Exiv2::ExifKey(tag), escapeCR);
}

QString QExiv2::xmpTagString(const char *tag, bool escapeCR) const
{
    return stringTag(d->xmpMetadata, Exiv2::XmpKey(tag), escapeCR);
}

QString QExiv2::xmpTagString(const QString &tag, bool escapeCR) const
{
    return stringTag(d->xmpMetadata, Exiv2::XmpKey(tag.toLatin1().constData()), escapeCR);
}
