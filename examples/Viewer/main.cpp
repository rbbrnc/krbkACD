#include <QApplication>
#include "MainWindow.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	QString fileName = argv[1];

	MainWindow w;
	w.setFile(fileName, true);
	w.show();

	return app.exec();
}
