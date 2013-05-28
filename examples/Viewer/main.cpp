#include <QApplication>
#include "ImageViewManager.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	QString fileName = argv[1];

	ImageViewManager w;
	w.setImage(fileName);
	w.show();

	return app.exec();
}
