#include <QApplication>
#include <QDebug>
#include "RegionEdit.h"
#include "QExiv2.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	if (argc < 2) {
		qDebug() << "missing filename";
		return -1;
	}

	QExiv2 *e = new QExiv2();
	if (!e->load(QString(argv[1]))) {
		delete e;
		return -1;
	}

	Widget w;
	w.show();
	return app.exec();
}
