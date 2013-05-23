#include <QApplication>
#include "ViewerWidget.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	QString fileName = argv[1];

	ViewerWidget w;
	w.setFile(fileName, true);
	w.show();

	return app.exec();
}
