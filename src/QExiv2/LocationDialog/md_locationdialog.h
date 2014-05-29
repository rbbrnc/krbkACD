#ifndef LOCATION_DIALOG_H
#define LOCATION_DIALOG_H

#include <QComboBox>
#include <QDialog>
#include <QGeoCoordinate>

#include "md_location.h"

class QExiv2;

namespace Ui {
	class LocationDialog;
}

class LocationDialog : public QDialog
{
	Q_OBJECT

	public:
		explicit LocationDialog(const QStringList &files, QWidget *parent = 0);
		~LocationDialog();

	private:
		bool getLocations(const QString &file);
		bool setLocation(const QString &file);
		bool locationIsEmpty() const;
        void enableWidgets(bool enable);

		void updateCountryNameComboBox(QComboBox *cb, int idx);

	public slots:
		virtual void accept();
		virtual void reject();

	private slots:
        void on_lcRegionCombo_currentIndexChanged(int index);
        void on_lcCountryCombo_currentIndexChanged(QString sel);
		void on_lcSetButton_clicked();

        void on_lsRegionCombo_currentIndexChanged(int index);
        void on_lsCountryCombo_currentIndexChanged(QString sel);
		void on_lsSetButton_clicked();

        void on_geolocationButton_clicked();

    private:
		Ui::LocationDialog *ui;

		QStringList m_fileList;
        QExiv2 *m_exiv2;

        MetadataLocation m_created;
        MetadataLocation m_shown;
        QGeoCoordinate   m_gpsCreated;
};

#endif
