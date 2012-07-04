#include <QtGui/QApplication>

#include "widget.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Widget w(argv[1]);
	w.show();
	return a.exec();
}
