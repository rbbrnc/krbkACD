#include <QDebug>

#include "QExiv2.h"
#include "QExiv2_p.h"

//////////////////////////////////////////////////////////////////////////////////////////
// IPTC Functions
//////////////////////////////////////////////////////////////////////////////////////////
bool QExiv2::isIptcWritable() const
{
	return d->isMetadataWritable(Exiv2::mdIptc);
}

bool QExiv2::clearIptc()
{
	try {
		d->iptcMetadata.clear();
		return true;

	} catch (Exiv2::Error& e) {
		d->error(__PRETTY_FUNCTION__, e);
	}

	return false;
}

QList<exifData> QExiv2::iptcDataList() const
{
	if (d->iptcMetadata.empty()) {
		QList<struct exifData>();
	}

	QList<struct exifData> lst;

	// fill list;
	Exiv2::IptcData::const_iterator end = d->iptcMetadata.end();
	for (Exiv2::IptcData::const_iterator md = d->iptcMetadata.begin(); md != end; ++md) {
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
