#ifndef DATETIMEDIALOG_H
#define DATETIMEDIALOG_H

#include <QDialog>
#include <QDateTime>

class QExiv2;

namespace Ui {
	class MetadataDateTimeDialog;
}

class MetadataDateTimeDialog : public QDialog
{
	Q_OBJECT

	public:
		explicit MetadataDateTimeDialog(const QStringList &files, QWidget *parent = 0);
		~MetadataDateTimeDialog();

	private:
        bool saveData();
        void enableWidgets(bool enable);

	public slots:
		virtual void accept();
		virtual void reject();

	private:
		Ui::MetadataDateTimeDialog *ui;
		QStringList m_fileList;
        QExiv2 *m_exiv2;
		QDateTime m_dateTime;
};

#endif
