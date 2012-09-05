#ifndef METADATA_GPS_H
#define METADATA_GPS_H

#include <QWidget>
#include <QFileInfo>

namespace Ui {
	class MetadataGPS;
}

class MetadataGPS : public QWidget
{
	Q_OBJECT

	public:
		MetadataGPS(const QFileInfo &fileInfo, QWidget *parent = 0);

	private:
		Ui::MetadataGPS *ui;
};

#endif
