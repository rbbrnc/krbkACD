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

	if (e->xmpHasRegionTags()) {
		MwgRegionList rl = e->xmpMwgRegionList();
		for (int i = 0; i < rl.count(); i++) {
			qDebug() << rl.at(i).stAreaBoundingRectF()
				 << rl.at(i).stArea()
				 << rl.at(i).name()
				 << rl.at(i).description();
		}
	} else {
		qDebug() << "No region tags found!";
	}

	delete e;
	return 0;
}
