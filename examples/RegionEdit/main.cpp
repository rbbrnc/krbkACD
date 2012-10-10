#include <QApplication>
#include <QDebug>
#include "RegionEdit.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

//	if (argc < 2) {
//		return -1;
//	}

	Widget w;
	w.show();
	return app.exec();
}
