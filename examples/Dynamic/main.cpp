#include <QApplication>
#include <QDebug>
#include "dynamic.h"

int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(Dynamic);

	QApplication app(argc, argv);
	Widget w;
	w.show();
	return app.exec();
}
