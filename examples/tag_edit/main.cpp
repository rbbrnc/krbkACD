#include <QtGui/QApplication>
#include "TagEditor.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	TagEditor w(argv[1]);
	w.show();

	return a.exec();
}
