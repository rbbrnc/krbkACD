#include <QDebug>

#include "QExiv2.h"
#include "QExiv2_p.h"

bool QExiv2::isXmpWritable() const
{
	return d->isMetadataWritable(Exiv2::mdXmp);
}

QList<exifData> QExiv2::xmpDataList() const
{
	if (d->xmpMetadata.empty()) {
		QList<struct exifData>();
	}

	QList<struct exifData> lst;

	// fill list;
	Exiv2::XmpData::const_iterator end = d->xmpMetadata.end();
	for (Exiv2::XmpData::const_iterator md = d->xmpMetadata.begin(); md != end; ++md) {
		struct exifData ed;

		ed.key      = QString(md->key().c_str());
		ed.family   = QString(md->familyName());
		ed.group    = QString(md->groupName().c_str());
		ed.tagName  = QString(md->tagName().c_str());
		ed.tag      = md->tag();
		ed.typeName = QString(md->typeName());
		ed.typeId   = md->typeId();
		ed.count    = md->count();
		ed.value    = QString::fromUtf8(md->value().toString().c_str());
		lst.append(ed);
	}

	return lst;
}

bool QExiv2::removeXmpTag(const char *tag)
{
	try {
		Exiv2::XmpKey xmpKey(tag);
		Exiv2::XmpData::iterator it = d->xmpMetadata.findKey(xmpKey);
		if (it != d->xmpMetadata.end()) {
			//qDebug() << __PRETTY_FUNCTION__ << "Erase key:" << tag;
			d->xmpMetadata.erase(it);
		}
		return true;

	} catch (Exiv2::Error &e) {
		d->error(QString("%1 Cannot remove tag '%2'").arg(__PRETTY_FUNCTION__).arg(tag), e);
	}

	return false;
}

void QExiv2::removeXmpBag(const char *tag, int tagNameSize)
{
	try {
		Exiv2::XmpData &md = d->xmpMetadata;
		Exiv2::XmpData::iterator it = md.begin();
		while (it != md.end()) {
			if (it->key().compare(0, tagNameSize, tag) == 0) {
				it = md.erase(it);
			} else {
				++it;
			}
		}
	} catch (Exiv2::Error& e) {
		d->error(QString("Cannot Erase Xmp Bag '%1'using Exiv2 ").arg(__func__), e);
	}
}


// Serialize the XMP data and output the XML XMP packet
QByteArray QExiv2::xmpPacket() const
{
	try {
		std::string xmpPacket;
		Exiv2::XmpData xmpData(d->xmpMetadata);

		if (0 != Exiv2::XmpParser::encode(xmpPacket, xmpData)) {
			//qDebug() << "Failed to serialize XMP data";
			throw Exiv2::Error(1, "Failed to serialize XMP data");
		} else {
			QByteArray ba(xmpPacket.c_str(), xmpPacket.size());
			qDebug() << ba;
			return ba;
		}

		// Cleanup
		Exiv2::XmpParser::terminate();

	} catch (Exiv2::Error &e) {
		d->error(__PRETTY_FUNCTION__, e);
	}

	return QByteArray();
}

QString QExiv2::xmpTagString(const char *tag, bool escapeCR) const
{
	try {
		Exiv2::XmpData xmpData(d->xmpMetadata);
		Exiv2::XmpKey key(tag);
		Exiv2::XmpData::const_iterator it = xmpData.findKey(key);
		if (it != xmpData.end()) {
			std::ostringstream os;
			os << *it;
			QString tagValue = QString::fromUtf8(os.str().c_str());

			if (escapeCR)
				tagValue.replace('\n', ' ');

			return tagValue;
		}
	} catch (Exiv2::Error &e) {
		d->error(QString("%1 Cannot find tag '%2'").arg(__PRETTY_FUNCTION__).arg(tag), e);
	}

	return QString();
}

QString QExiv2::xmpTagString(const QString &tag, bool escapeCR) const
{
	return xmpTagString(tag.toLatin1().constData(), escapeCR);
}

bool QExiv2::setXmpTagString(const char *tag, const QString &value)
{
	if (value.isEmpty()) {
		return removeXmpTag(tag);
	}

	try {
		const std::string &txt(value.toUtf8().constData());
		Exiv2::Value::AutoPtr val = Exiv2::Value::create(Exiv2::xmpText);
		val->read(txt);
		d->xmpMetadata[tag].setValue(val.get());
		return true;
	} catch (Exiv2::Error &e) {
		d->error(QString("%1 Cannot set tag '%2'").arg(__PRETTY_FUNCTION__).arg(tag), e);
	}

	return false;
}

bool QExiv2::setXmpTagString(const QString &tag, const QString& value)
{
	return setXmpTagString(tag.toLatin1().constData(), value);
}

//****************************************************************************
// XMP Bag
//****************************************************************************
QStringList QExiv2::xmpTagStringBag(const char *tag, bool escapeCR) const
{
	try {
		Exiv2::XmpData xmpData(d->xmpMetadata);
		Exiv2::XmpKey key(tag);
		Exiv2::XmpData::const_iterator it = xmpData.findKey(key);
		if (it != xmpData.end()) {
			//qDebug() << __func__ << "TypeID: 0x" << QString::number(it->typeId(), 16);
			if (it->typeId() == Exiv2::xmpBag) {
				QStringList bag;
				for (int i = 0; i < it->count(); i++) {
					std::ostringstream os;
					os << it->toString(i);
					QString bagValue = QString::fromUtf8(os.str().c_str());
					if (escapeCR) {
						bagValue.replace('\n', ' ');
					}
					bag.append(bagValue);
				}
				return bag;
			}
		}
	} catch (Exiv2::Error &e) {
		d->error(QString("%1 Cannot find tag '%2'").arg(__PRETTY_FUNCTION__).arg(tag), e);
	}

	return QStringList();
}

bool QExiv2::setXmpTagStringBag(const char *tag, const QStringList &bag)
{
	if (bag.isEmpty()) {
		return removeXmpTag(tag);
	}

	try {
		QStringList list = bag;
		Exiv2::Value::AutoPtr xmpTxtBag = Exiv2::Value::create(Exiv2::xmpBag);

		for (QStringList::const_iterator it = list.constBegin(); it != list.constEnd(); ++it) {
			const std::string &txt((*it).toUtf8().constData());
			xmpTxtBag->read(txt);
		}
		d->xmpMetadata[tag].setValue(xmpTxtBag.get());
		return true;
	} catch (Exiv2::Error &e) {
		d->error(QString("%1 Cannot find tag '%2'").arg(__PRETTY_FUNCTION__).arg(tag), e);
	}

	return false;
}

bool QExiv2::setXmpTagBag(const char *tag)
{
	try {
		Exiv2::Value::AutoPtr xmpTxtBag = Exiv2::Value::create(Exiv2::xmpBag);
		d->xmpMetadata[tag].setValue(xmpTxtBag.get());
		return true;
	} catch (Exiv2::Error &e) {
		d->error(QString("%1 Cannot set tag '%2'").arg(__PRETTY_FUNCTION__).arg(tag), e);
	}
	return false;
}

//****************************************************************************
// XMP Seq
//****************************************************************************
QStringList QExiv2::xmpTagStringSeq(const char *tag, bool escapeCR) const
{
	try {
		Exiv2::XmpData xmpData(d->xmpMetadata);
		Exiv2::XmpKey key(tag);
		Exiv2::XmpData::const_iterator it = xmpData.findKey(key);
		if (it != xmpData.end()) {
			if (it->typeId() == Exiv2::xmpSeq) {
				QStringList seq;
				for (int i = 0; i < it->count(); i++) {
					std::ostringstream os;
					os << it->toString(i);
					QString seqValue = QString::fromUtf8(os.str().c_str());

					if (escapeCR)
						seqValue.replace('\n', ' ');

					seq.append(seqValue);
				}
				qDebug() << "XMP String Seq (" << tag << "): " << seq;
				return seq;
			}
		}
	} catch (Exiv2::Error &e) {
		d->error(QString("%1 Cannot find tag '%2'").arg(__PRETTY_FUNCTION__).arg(tag), e);
	}

	return QStringList();
}

bool QExiv2::setXmpTagStringSeq(const char *tag, const QStringList &seq)
{
	if (seq.isEmpty()) {
		return removeXmpTag(tag);
	}

	try {
		const QStringList list = seq;
		Exiv2::Value::AutoPtr xmpTxtSeq = Exiv2::Value::create(Exiv2::xmpSeq);

		for (QStringList::const_iterator it = list.constBegin(); it != list.constEnd(); ++it ) {
			const std::string &txt((*it).toUtf8().constData());
			xmpTxtSeq->read(txt);
		}
		d->xmpMetadata[tag].setValue(xmpTxtSeq.get());
		return true;

	} catch (Exiv2::Error &e) {
		d->error(QString("%1 Cannot set tag '%2'").arg(__PRETTY_FUNCTION__).arg(tag), e);
	}

	return false;
}

//****************************************************************************
// XMP LangAlt Tag
//****************************************************************************
QString QExiv2::xmpTagStringLangAlt(const char *tag, const QString &langAlt, bool escapeCR)
{
	try {
		Exiv2::XmpData xmpData(d->xmpMetadata);
		Exiv2::XmpKey key(tag);
	        for (Exiv2::XmpData::iterator it = xmpData.begin(); it != xmpData.end(); ++it) {
			if ((it->key() == tag) && (it->typeId() == Exiv2::langAlt)) {
				for (int i = 0; i < it->count(); i++) {
					std::ostringstream os;
					os << it->toString(i);
					QString lang;
					QString tagValue = QString::fromUtf8(os.str().c_str());
					//qDebug() << __func__ << tagValue;
					if (langAlt == lang) {
						if (escapeCR) {
							tagValue.replace('\n', ' ');
						}
						return tagValue;
					}
				}
			}
		}
	} catch (Exiv2::Error& e) {
		d->error(QString("%1 Cannot find tag '%2'").arg(__PRETTY_FUNCTION__).arg(tag), e);
	}

	return QString();
}

bool QExiv2::setXmpTagStringLangAlt(const char *tag, const QString &value, const QString &langAlt)
{
	QString txtLangAlt;

	if (langAlt.isEmpty()) {
		// Default alternative language.
		txtLangAlt = QString("lang=x-default %1").arg(value);
	} else {
		txtLangAlt = QString("lang=%1 %2").arg(langAlt).arg(value);
	}

	try {
		const std::string &txt(txtLangAlt.toUtf8().constData());
		Exiv2::Value::AutoPtr xmpTxtVal = Exiv2::Value::create(Exiv2::langAlt);
		xmpTxtVal->read(txt);
		removeXmpTag(tag);
		d->xmpMetadata.add(Exiv2::XmpKey(tag), xmpTxtVal.get());
		return true;
	} catch (Exiv2::Error &e) {
		d->error(QString("%1 Cannot add tag '%2'").arg(__PRETTY_FUNCTION__).arg(tag), e);
	}
	return false;
}
