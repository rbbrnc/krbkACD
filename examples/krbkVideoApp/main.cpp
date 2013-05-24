#include <QtGui>
#include <QDebug>

#include "AvManager.h"

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
	QApplication app(argc, argv);

	if (argc < 2) {
		qDebug() << "Missing File";
		return -1;
	}

	QString fileName = argv[1];
	AvManager *w = new AvManager(fileName);

	w->setStyleSheet(style(":/fancySlider.qss"));
	w->show();

	return app.exec();
}
