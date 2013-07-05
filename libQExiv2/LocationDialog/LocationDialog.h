#ifndef LOCATION_DIALOG_H
#define LOCATION_DIALOG_H

#include <QtGui>
#include <QDialog>

class Location
{
	public:
		Location() {}
		~Location() {}

		bool isEmpty() const;

		QString worldRegion;
		QString countryName;
		QString countryCode;
		QString provinceState;
		QString city;
		QString sublocation;
};

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

		void updateCountryNameComboBox(QComboBox *cb, int idx);

	public slots:
		virtual void accept();
		virtual void reject();

	private slots:
		void on_lcWorldRegionComboBox_currentIndexChanged(int index);
		void on_lcCountryNameComboBox_currentIndexChanged(QString sel);
		void on_lcSetButton_clicked();

		void on_lsWorldRegionComboBox_currentIndexChanged(int index);
		void on_lsCountryNameComboBox_currentIndexChanged(QString sel);
		void on_lsSetButton_clicked();

	private:
		Ui::LocationDialog *ui;

		QStringList m_fileList;

		Location m_locationCreated;
		Location m_locationShown;
};

#endif
