#include <QApplication>
#include <QDebug>
#include "RenameDialog.h"

int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(Dynamic);

	QApplication app(argc, argv);

	RenameDialog dlg(argv[1]);

	if (dlg.exec() == QDialog::Accepted) {
		qDebug() << dlg.newFileName();
	}

	return 0;
}
