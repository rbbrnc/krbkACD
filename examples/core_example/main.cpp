#include <QCoreApplication>
#include <QFile>

#include <QDebug>
#include <QStringList>

#include "QExiv2.h"

#define LOAD_FROM_FILE 1


int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

#if LOAD_FROM_FILE
	// Se il file non e' leggibile l'app crasha!
	// workaround: usare il costruttore nullo controllare il valore
	// ritornato dalla load()
	//QExiv2 *e = new QExiv2(QString(argv[1]));

	qDebug() << "LOAD FROM FILE #######################################";
	QExiv2 *e = new QExiv2();
	if (!e->load(QString(argv[1]))) {
		delete e;
		return -1;
	}
#else
	qDebug() << "LOAD FROM DATA #######################################";
	QFile file(argv[1]);
	if (!file.open(QIODevice::ReadOnly)) {
		return -1;
	}

	QByteArray ba = file.readAll();
	if (ba.isEmpty()) {
		return -1;
	}

	QExiv2 *e = new QExiv2();
	if (!e->loadFromData(ba)) {
		delete e;
		return -1;
	}
#endif

#if 0

	QStringList sl;
	sl << "A" << "B" << "C";

	QMap<QString, QString> tagMap;
	tagMap.insert("mwg-rs:Name", "n");
	tagMap.insert("mwg-rs:Type", "t");
	tagMap.insert("mwg-rs:Description", "d");
	tagMap.insert("mwg-rs:Area/stArea:x", "x");
	tagMap.insert("mwg-rs:Area/stArea:y", "y");
	tagMap.insert("mwg-rs:Area/stArea:w", "w");
	tagMap.insert("mwg-rs:Area/stArea:h", "h");
	tagMap.insert("mwg-rs:Area/stArea:unit", "u");
#if 0
	tv.read("");                                         // Clear the value
	tv.setXmpArrayType(Exiv2::XmpValue::xaBag);
	xmpData.add(Exiv2::XmpKey("Xmp.xmpBJ.JobRef"), &tv); // Set the array type.

	tv.setXmpArrayType(Exiv2::XmpValue::xaNone);
	tv.read("Birthday party");
	xmpData.add(Exiv2::XmpKey("Xmp.xmpBJ.JobRef[1]/stJob:name"), &tv);
	tv.read("Photographer");
	xmpData.add(Exiv2::XmpKey("Xmp.xmpBJ.JobRef[1]/stJob:role"), &tv);

	tv.read("Wedding ceremony");
	xmpData.add(Exiv2::XmpKey("Xmp.xmpBJ.JobRef[2]/stJob:name"), &tv);
	tv.read("Best man");
	xmpData.add(Exiv2::XmpKey("Xmp.xmpBJ.JobRef[2]/stJob:role"), &tv);
#endif
	e->setXmpTagString("Xmp.mwg-rs.Regions/mwg-rs:RegionList", "");

	//e->setXmpTagStringBag("Xmp.mwg-rs.Regions/mwg-rs:RegionList/mwg-rs:RegionList[%1]", sl);
	e->save();
	return 0;
#endif

	if (e->hasXmpRegionTag()) {
		QList<XmpRegion> tl = e->xmpRegionList();
		for (int i = 0; i < tl.size(); i++) {
			tl.at(i).debug();
		}
	} else {
		qDebug() << "No region tags found!";
	}

	delete e;
	return 0;
}
