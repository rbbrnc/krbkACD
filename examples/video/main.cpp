#include <QtGui>
#include <QDebug>

#include "AvManager.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);

	if (argc < 2) {
		qDebug() << "Missing File";
		return -1;
	}

	QString fileName = argv[1];
	AvManager *w = new AvManager(fileName);
	w->show();

	return app.exec();
}
