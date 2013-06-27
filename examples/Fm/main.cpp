#include <QApplication>
#include <QDir>

#include "MainWindow.h"

int main(int argc, char **argv)
{
	QApplication a(argc, argv);

	if (argc > 1) {
		QDir::setCurrent(QString(argv[1]));
	}

	MainWindow w;
	w.show();
	return a.exec();
}
