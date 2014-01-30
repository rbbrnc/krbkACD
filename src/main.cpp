#include <QApplication>
#include <QDir>

#include "mainwindow.h"

#ifdef USE_CUSTOM_STYLE
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
#endif

int main(int argc, char **argv)
{
	QApplication a(argc, argv);

	if (argc > 1) {
		QDir::setCurrent(QString(argv[1]));
	}
	MainWindow w;

#ifdef USE_CUSTOM_STYLE
	w.setStyleSheet(style(":/darkorange.qss"));
#endif

	w.show();

	return a.exec();
}
