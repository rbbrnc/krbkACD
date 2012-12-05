#include <QtGui>
#include "MetadataTreeViewPage.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QString file(argv[1]);

	MetadataTreeViewPage view;
	view.setFile(file);
	view.setWindowTitle(QObject::tr("Metadata Tree Model View"));
	view.show();
	return app.exec();
}
