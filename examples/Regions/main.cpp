#include <QtGui>

#include "QExiv2.h"
#include "RegionEditManager.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QString file(argv[1]);
	RegionEditManager manager;
	manager.setFile(file);
	manager.show();
	return app.exec();
}
