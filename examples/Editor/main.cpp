#include <QApplication>

#include "MetadataDialog.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QString fileName;

	if (argc >= 2)
		fileName = argv[1];
	else
		fileName = ".";

	MetadataDialog dlg(fileName);
	return dlg.exec();
}
