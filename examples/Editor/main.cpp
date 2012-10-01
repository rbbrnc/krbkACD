#include <QApplication>

#include "MetadataDialog.h"
#include "QExiv2.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QString fileName;

	if (argc >= 2)
		fileName = argv[1];
	else
		fileName = ".";

	QExiv2 *metadata = new QExiv2();
	if (!metadata->load(fileName)) {
		delete metadata;
		metadata = 0;
	}

	MetadataDialog dlg(fileName, metadata);
	return dlg.exec();
}
