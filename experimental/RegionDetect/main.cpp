#include <QCoreApplication>
#include <QDebug>

#include "detect.h"
#include "mwgRegion.h"
#include "mwgRegionList.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	if (argc < 2) {
		qFatal("No arg");
		return -1;
	}

	ObjectDetect *o = new ObjectDetect();
	o->setSource(argv[1]);

	int count = o->detect();

	if (count > 0) {
		QList<QRect> regions = o->objects();

		MwgRegionList rl;

		int imgW = o->sourceWidth();
		int imgH = o->sourceHeight();

		for (int i = 0; i < regions.count(); i++) {
			rl.append(MwgRegion(regions.at(i), QSize(imgW, imgH), MwgRegion::Face));
		}

		qDebug() << "Objects Found:" << rl;
	} else {
		qDebug() << "No Objects Found";
	}

	delete o;

	return 0;
}
