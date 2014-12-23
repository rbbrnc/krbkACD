#include <QDebug>

#include "QExiv2.h"
#include "qexiv2_p.h"


bool QExiv2::removeXmpTag(const char *tag)
{
	try {
        Exiv2::XmpData::iterator it = d->xmpMetadata.findKey(Exiv2::XmpKey(tag));
		if (it != d->xmpMetadata.end()) {
			d->xmpMetadata.erase(it);
		}
		return true;

	} catch (Exiv2::Error &e) {
        d->error(QString("%1 Cannot remove tag '%2'").arg(__func__).arg(tag), e);
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
    } catch (Exiv2::Error &e) {
		d->error(QString("Cannot Erase Xmp Bag '%1'using Exiv2 ").arg(__func__), e);
	}
}

#ifdef ENABLE_XMP_SERIALIZE
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
        d->error(__func__, e);
	}

	return QByteArray();
}
#endif

bool QExiv2::setXmpTagString(const char *tag, const QString &value)
{
	if (value.isEmpty()) {
		return removeXmpTag(tag);
	}

	try {
		Exiv2::Value::AutoPtr val = Exiv2::Value::create(Exiv2::xmpText);
        val->read(value.toStdString());
		d->xmpMetadata[tag].setValue(val.get());
		return true;
	} catch (Exiv2::Error &e) {
        d->error(QString("%1 Cannot set tag '%2'").arg(__func__).arg(tag), e);
	}

	return false;
}

bool QExiv2::setXmpTagString(const QString &tag, const QString &value)
{
	return setXmpTagString(tag.toLatin1().constData(), value);
}

bool QExiv2::setXmpStringList(const char *tag, const QStringList &list, int typeId)
{
    if (list.isEmpty()) {
        return removeXmpTag(tag);
    }

    try {
        Exiv2::Value::AutoPtr vptr = Exiv2::Value::create(static_cast<Exiv2::TypeId>(typeId));

        for (QStringList::const_iterator it = list.constBegin(); it != list.constEnd(); ++it ) {
            vptr->read((*it).toStdString());
        }

        d->xmpMetadata[tag].setValue(vptr.get());
        return true;

    } catch (Exiv2::Error &e) {
        d->error(QString("%1 Cannot set tag '%2'").arg(__func__).arg(tag), e);
    }

    return false;
}

QStringList QExiv2::xmpStringList(const char *tag, bool escapeCR) const
{
	QStringList tagList = QStringList();

	try {
		Exiv2::XmpData xmpData(d->xmpMetadata);
        Exiv2::XmpData::const_iterator it = xmpData.findKey(Exiv2::XmpKey(tag));
		if (it != xmpData.end()) {
			//qDebug() << __func__ << "TypeID: 0x" << QString::number(it->typeId(), 16);
			switch (it->typeId()) {
			case Exiv2::xmpBag:
			case Exiv2::xmpSeq:
				for (int i = 0; i < it->count(); i++) {
                    QString value = QString::fromStdString(it->toString(i));
					if (escapeCR) {
                        value.replace('\n', ' ');
					}
                    tagList.append(value);
				}
			default:
				break;
			}
		}
	} catch (Exiv2::Error &e) {
        d->error(QString("%1 Cannot find tag '%2'").arg(__func__).arg(tag), e);
	}

    return tagList;
}

//****************************************************************************
// XMP Bag
//****************************************************************************
QStringList QExiv2::xmpTagStringBag(const char *tag, bool escapeCR) const
{
    return xmpStringList(tag, escapeCR);
}

bool QExiv2::setXmpBag(const char *tag, const QStringList &bag)
{
    return setXmpStringList(tag, bag, Exiv2::xmpBag);
}

bool QExiv2::setXmpTagBag(const char *tag)
{
	try {
        Exiv2::Value::AutoPtr valPtr = Exiv2::Value::create(Exiv2::xmpBag);
        d->xmpMetadata[tag].setValue(valPtr.get());
		return true;
	} catch (Exiv2::Error &e) {
        d->error(QString("%1 Cannot set tag '%2'").arg(__func__).arg(tag), e);
	}
	return false;
}

//****************************************************************************
// XMP Seq
//****************************************************************************
QStringList QExiv2::xmpTagStringSeq(const char *tag, bool escapeCR) const
{
    return xmpStringList(tag, escapeCR);
}

bool QExiv2::setXmpTagStringSeq(const char *tag, const QStringList &seq)
{
    return setXmpStringList(tag, seq, Exiv2::xmpSeq);
}

//****************************************************************************
// XMP LangAlt Tag
//****************************************************************************
QString QExiv2::xmpLangAlt(const char *tag, const QString &langAlt, bool escapeCR)
{
	try {
		Exiv2::XmpData xmpData(d->xmpMetadata);
        for (Exiv2::XmpData::iterator it = xmpData.begin(); it != xmpData.end(); ++it) {
			if ((it->key() == tag) && (it->typeId() == Exiv2::langAlt)) {
				for (int i = 0; i < it->count(); i++) {
                    QString value = QString::fromStdString(it->toString(i));

					QString lang;
					if (langAlt == lang) {
						if (escapeCR) {
                            value.replace('\n', ' ');
						}
                        return value;
					}
				}
			}
		}
    } catch (Exiv2::Error &e) {
        d->error(QString("%1 Cannot find tag '%2'").arg(__func__).arg(tag), e);
	}

	return QString();
}

bool QExiv2::setXmpLangAlt(const char *tag, const QString &value, const QString &langAlt)
{
    const std::string txt = QString("lang=%1 %2").arg(langAlt).arg(value).toStdString();

	try {
		Exiv2::Value::AutoPtr xmpTxtVal = Exiv2::Value::create(Exiv2::langAlt);
        xmpTxtVal->read(txt);
		removeXmpTag(tag);
		d->xmpMetadata.add(Exiv2::XmpKey(tag), xmpTxtVal.get());
		return true;
	} catch (Exiv2::Error &e) {
        d->error(QString("%1 Cannot add tag '%2'").arg(__func__).arg(tag), e);
	}
	return false;
}

bool QExiv2::xmpRegisterNamespace(const QString &uri, const QString &prefix)
{
	try {
		QString ns = uri;
		if (!uri.endsWith('/')) {
			ns.append('/');
		}
		Exiv2::XmpProperties::registerNs(ns.toLatin1().constData(), prefix.toLatin1().constData());
		return true;
	} catch (Exiv2::Error &e) {
        d->error(QString("%1 Cannot register namespace '%2'").arg(__func__).arg(uri), e);
	}
	return false;
}

