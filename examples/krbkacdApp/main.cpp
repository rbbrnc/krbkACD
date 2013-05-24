#include <QApplication>
#include <QDir>

#include "MainWindow.h"


const QString style(const QString &qssFileName)
{
	if (qssFileName.isEmpty()) {
		return QString();
	}

	QFile file(qssFileName);
	file.open(QFile::ReadOnly);
	QString styleSheet = QLatin1String(file.readAll());
	file.close();
	return styleSheet;
}

int main(int argc, char **argv)
{
	Q_INIT_RESOURCE(RenameDialog);

	QApplication a(argc, argv);

	if (argc > 1) {
		QDir::setCurrent(QString(argv[1]));
	}
	MainWindow w;

	//w.setStyleSheet(style(":/darkTheme.qss"));
	w.setStyleSheet(style(":/darkorange.qss"));
	w.show();

	return a.exec();
}
