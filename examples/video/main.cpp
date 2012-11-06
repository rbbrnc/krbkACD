#include <QtGui>
#include <QDebug>

#include "VideoWidget.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);

	if (argc < 2) {
		qDebug() << "Missing File";
		return -1;
	}

	QString fileName = argv[1];
	VideoWidget *w = new VideoWidget(fileName);
	w->show();

	return app.exec();
}
