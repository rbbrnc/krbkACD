#include <QtGui>

#include "QExiv2.h"
#include "RegionEditManager.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	RegionEditManager manager;

	if (argc >= 2) {
		QString fileName;
		fileName = argv[1];
		manager.setFile(fileName);
	} else {
		manager.setFile(QString());
	}

	manager.show();
	return app.exec();
}
