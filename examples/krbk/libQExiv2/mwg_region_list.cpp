#include <QDebug>

//#include "detect.h"
#include "mwg_region_list.h"

void MwgRegionList::detectFromFile(const QString &filePath)
{
#if 0
	ObjectDetect *o = new ObjectDetect();
	o->setSource(filePath.toLatin1().data());

	int count = o->detect();

	if (count > 0) {
		QList<QRect> regions = o->objects();

		int imgW = o->sourceWidth();
		int imgH = o->sourceHeight();

		for (int i = 0; i < regions.count(); i++) {
			append(MwgRegion(regions.at(i), QSize(imgW, imgH), MwgRegion::Face));
		}
	}

	delete o;
#endif
}
