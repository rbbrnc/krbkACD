#ifndef METADATA_EDIT_PAGE_H
#define METADATA_EDIT_PAGE_H

#include <QWidget>

namespace Ui {
	class MetadataEditPage;
}

class QExiv2;

class MetadataEditPage : public QWidget
{
	Q_OBJECT

	public:
		explicit MetadataEditPage(QWidget *parent = 0);
		~MetadataEditPage();

		void setFile(const QString &file);
        bool saveMetadata();

    private:
        void getDateTime();
        void getLocations();

    private slots:
        void on_locationEditButton_clicked();
        void on_datetimeEditButton_clicked();
        void on_keywordsEditButton_clicked();
        void on_ratingSpinBox_valueChanged(double);

        void on_tabWidget_currentChanged(int index);


        void on_reverseGeocodingButton_clicked();

private:
		Ui::MetadataEditPage *ui;
        QExiv2 *m_exiv2;
        QString m_fileName;
        bool m_updateRating;
};
#endif
