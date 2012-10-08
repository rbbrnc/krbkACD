#include <QApplication>
#include <QDebug>
#include "SingleFileRenameDialog.h"
#include "MultiFileRenameDialog.h"

int main(int argc, char *argv[])
{
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
		SingleFileRenameDialog dlg(files.at(0));
		return dlg.exec();
	} else if (n > 1) {
		MultiFileRenameDialog dlg(files);
		return dlg.exec();
	} else {
		qDebug() << __PRETTY_FUNCTION__ << "Error 3";
		return -1;
	}
}
