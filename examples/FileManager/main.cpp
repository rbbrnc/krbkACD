#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(RenameDialog);

	QApplication a(argc, argv);


	if (argc > 1) {
		/*bool*/ QDir::setCurrent(QString(argv[1]));
	}

	qDebug() << "App CWD:" << QDir::current();

	MainWindow w;
	w.resize(300, 700);
	w.show();

	return a.exec();
}
