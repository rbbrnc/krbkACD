#ifndef METADATA_LOCATION_H
#define METADATA_LOCATION_H

#include <QWidget>
#include <QFileInfo>

namespace Ui {
	class MetadataLocation;
}

class QExiv2;
class MetadataLocation : public QWidget
{
	Q_OBJECT

	public:
		MetadataLocation(QExiv2 *metadata, QWidget *parent = 0);

	private:
		Ui::MetadataLocation *ui;
		QExiv2 *m_md;
};

#endif
