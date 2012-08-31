#include <QtGui/QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(SpotlightWidget);

	QApplication a(argc, argv);
	MainWindow w(argv[1]);
	w.show();

	return a.exec();
}
