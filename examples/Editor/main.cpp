#include <QApplication>

#include "MetadataEditForm.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QString fileName = argv[1];

	MetadataEditForm frm;

	frm.setFile(fileName);
	frm.show();
	return app.exec();
}
