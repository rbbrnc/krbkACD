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

    private:
        void getDateTime();
        void getLocations();

    private slots:
        void on_locationEditButton_clicked();
        void on_datetimeEditButton_clicked();

private:
		Ui::MetadataEditPage *ui;
        QExiv2 *m_exiv2;
        QString m_fileName;
};
#endif
