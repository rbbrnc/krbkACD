#include <QCoreApplication>
#include <QDebug>

#include "detect.h"
#include "mwgRegionList.h"

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
			regionList->append(MwgRegion(regions.at(i), QSize(imgW, imgH), MwgRegion::Face));
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

	if (rl.count() > 0) {
		qDebug() << "Objects Found:" << rl;
	} else {
		qDebug() << "No Objects Found";
	}

	return 0;
}
