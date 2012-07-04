#include <QCoreApplication>
#include <QFile>

#include <QDebug>

#include "QExiv2.h"
#include "PTag.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

#if 0
	// Se il file non e' leggibile l'app crasha!
	// workaround: usare il costruttore nullo controllare il valore
	// ritornato dalla load()
	QExiv2 *e = new QExiv2(QString(argv[1]));
#else
	qDebug() << "LOAD FROM FILE #######################################";
	QExiv2 *e = new QExiv2();
	if (!e->load(QString(argv[1]))) {
		delete e;
		return -1;
	}
#endif

#if 0
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


//	PTag p = e->xmpPTag();
//	p.debug();

//	QStringList sl = e->xmpTagStringBag("Xmp.MP.RegionInfo/MPRI:Regions", false);
//	qDebug() << sl;

	QList<PTag> tl = e->xmpPTags();
	for (int i = 0; i < tl.size(); i++)
		tl.at(i).debug();
#if 0
	if (e->isImgCommentWritable()) {
		qDebug() << "COM writable";
		if (e->hasComment()) {
			qDebug() << "COM:" << e->imgComment();
			qDebug() << "Now Delete COM";
			e->clearImgComment();
			if (e->hasComment()) {
				qDebug() << "WHAT!? COM:" << e->imgComment();
			}
		} else {
			e->setImgComment("KRBK Added this Comment");
			if (e->hasComment()) {
				qDebug() << "COM:" << e->imgComment();
			}
			qDebug() << "SAVE";
			if (e->save()) {
				qDebug() << "SAVE Ok";
			} else {
				qDebug() << "SAVE Err.";
			}
		}
	} else {
		qDebug() << "COM not writable";
	}
#else
	if (e->isImgCommentWritable()) {
		if (e->hasComment()) {
			qDebug() << "old COM:" << e->imgComment();
		}

		e->setImgComment("KRBK Added this Comment");
		qDebug() << "new COM:" << e->imgComment();

		if (e->save()) {
			qDebug() << "SAVE Ok";
		} else {
			qDebug() << "SAVE Err.";
		}
	} else {
		qDebug() << "COM not writable";
	}
#endif
	delete e;

	return 0;
}
