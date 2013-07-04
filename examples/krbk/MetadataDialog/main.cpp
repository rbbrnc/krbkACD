#include <QApplication>

#include "socialmetadatadialog.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QStringList files;
	files << argv[1];

	SocialMetadataDialog frm(files);

	frm.show();
	return app.exec();
}
