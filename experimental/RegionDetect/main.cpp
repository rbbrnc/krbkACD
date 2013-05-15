#include <QCoreApplication>
#include <QDebug>

#include "mwgRegionList.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	if (argc < 2) {
		qFatal("No arg");
		return -1;
	}

	MwgRegionList rl;
	rl.detectFromFile(QString(argv[1]));

	if (rl.count() > 0) {
		qDebug() << "Objects Found:" << rl;
	} else {
		qDebug() << "No Objects Found";
	}

	return 0;
}
