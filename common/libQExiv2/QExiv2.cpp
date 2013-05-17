#include <QFile>
#include <QFileInfo>
#include <QDebug>

#include "QExiv2.h"
#include "QExiv2_p.h"

//

QExiv2::QExiv2() : d(new QExiv2DataPrivate)
{
}

QExiv2::QExiv2(const QExiv2& other) : d(other.d)
{
}

QExiv2::QExiv2(const QString& filePath) : d(new QExiv2DataPrivate)
{
	load(filePath);
}

QExiv2::~QExiv2()
{
}

QExiv2& QExiv2::operator=(const QExiv2& other)
{
	d = other.d;
	return *this;
}

bool QExiv2::isValid() const
{
	return d->metadataValid;
}

bool QExiv2::loadFromData(const QByteArray& data)
{
	if (data.isEmpty()) {
		return false;
	}

	try {
		d->image = Exiv2::ImageFactory::open((Exiv2::byte *) data.constData(), data.size());
	} catch (Exiv2::Error &e) {
		d->printExiv2ExceptionError("Cannot load metadata from Data using Exiv2", e);
		return false;
	}

	return d->readMetadata();
}

bool QExiv2::load(const QString& filePath)
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

	try {
		d->image = Exiv2::ImageFactory::open((const char *)(QFile::encodeName(filePath)));
	} catch (Exiv2::Error &e) {
		d->printExiv2ExceptionError("Cannot load metadata from File using Exiv2", e);
		return false;
	}

	return d->readMetadata();
}

// Se il file non ha i permessi in scrittura scatta l'exception
bool QExiv2::save()
{
	if (!isValid()) {
		return false;
	}

	try {
		bool update = false;
		if (isImgCommentWritable()) {
			if (d->image->comment() != d->imageComment) {
				d->image->setComment(d->imageComment);
				update = true;
			} else {
				qDebug() << __PRETTY_FUNCTION__ << "Same Image Comment";
			}
		}

		if (isXmpWritable()) {
			//d->xmpMetadata.sortByKey();
			d->image->setXmpData(d->xmpMetadata);
			update = true;
		}

		if (update) {
			qDebug() << __PRETTY_FUNCTION__ << "write Metadata";
			d->image->writeMetadata();
		}

	       return true;

	} catch (Exiv2::Error& e) {
		d->printExiv2ExceptionError("Cannot save metadata using Exiv2", e);
	}

	return false;
}
