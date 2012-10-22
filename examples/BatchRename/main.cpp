#include <QApplication>
#include <QDebug>
#include "RenameDialog.h"
#include "BatchRenameDialog.h"

int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(RenameDialog);

	QApplication app(argc, argv);

	if (argc < 2) {
		qDebug() << __PRETTY_FUNCTION__ << "Missing arg[s]";
		return -1;
	}

	QStringList files;
	for (int i = 1; i < argc; i++) {
		files << argv[i];
	}

	if (files.isEmpty()) {
		qDebug() << __PRETTY_FUNCTION__ << "Empty file list!";
		return -1;
	}

	qDebug() << "In:" << files;
	int n = files.count();

	if (n == 1) {
		RenameDialog dlg(files.at(0));
		if (dlg.exec() == QDialog::Accepted) {
			qDebug() << dlg.newFileName();
		} else {
			qDebug() << "Single Reject";
		}
	} else if (n > 1) {
		BatchRenameDialog dlg(files);
		if (dlg.exec() == QDialog::Accepted) {
			qDebug() << dlg.newFileNames();
		} else {
			qDebug() << "Multi Reject";
		}
	} else {
		qDebug() << __PRETTY_FUNCTION__ << "Error 3";
		return -1;
	}

	return 0;
}
