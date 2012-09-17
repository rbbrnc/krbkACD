#include <QtGui>

#include "QExiv2.h"
#include "MetadataTreeModel.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QString file(argv[1]);

	QExiv2 *exiv2 = new QExiv2();
	if (exiv2->load(file)) {
		MetadataTreeModel model(exiv2);
		QTreeView view;
		view.setUniformRowHeights(true);
		view.setModel(&model);
		view.setWindowTitle(QObject::tr("Metadata Tree Model"));

		view.hideColumn(7); // hide key column

		view.show();
		return app.exec();
	}
	return -1;
}
