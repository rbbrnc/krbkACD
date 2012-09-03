#include <QtGui>

#include "QExiv2.h"
#include "MetadataModel.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QString file(argv[1]);

	QExiv2 *exiv2 = new QExiv2();
	if (exiv2->load(file)) {
		MetadataModel model(exiv2);
		QTreeView view;
		view.setModel(&model);
		view.setWindowTitle(QObject::tr("Simple Tree Model"));
		view.show();
		return app.exec();
	}
	return -1;
}
