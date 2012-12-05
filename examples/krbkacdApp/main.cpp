#include <QApplication>
#include "MainWindow.h"
#include <QDir>

int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(RenameDialog);

	QApplication a(argc, argv);

	if (argc > 1) {
		QDir::setCurrent(QString(argv[1]));
	}

	MainWindow w;
	w.show();

	return a.exec();
}
