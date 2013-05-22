#include <QCoreApplication>
#include <QDebug>

#include "detect.h"
#include "QExiv2.h"

void detectFromFile(MwgRegionList *regionList, const QString &filePath)
{
	ObjectDetect *o = new ObjectDetect();
	o->setSource(filePath.toLatin1().data());

	int count = o->detect();

	if (count > 0) {
		QList<QRect> regions = o->objects();

		int imgW = o->sourceWidth();
		int imgH = o->sourceHeight();

		for (int i = 0; i < regions.count(); i++) {
			MwgRegion r(regions.at(i), QSizeF(imgW, imgH), false);
			r.setType(MwgRs::Face);
			regionList->append(r);
		}
	}

	delete o;
}
int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	if (argc < 2) {
		qFatal("No arg");
		return -1;
	}

	MwgRegionList rl;
	detectFromFile(&rl, QString(argv[1]));

	QExiv2 *e = new QExiv2();
	if (!e->load(QString(argv[1]))) {
		delete e;
		return -1;
	}

	if (rl.count() > 0) {
		qDebug() << "Objects Found:" << rl;
		e->xmpSetMwgRegionList(rl);
		e->save();
	} else {
		qDebug() << "No Objects Found";
	}


	delete e;
	return 0;
}
