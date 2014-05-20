#ifndef DATETIMEDIALOG_H
#define DATETIMEDIALOG_H

#include <QDialog>
#include <QDateTime>

namespace Ui {
	class MetadataDateTimeDialog;
}

class MetadataDateTimeDialog : public QDialog
{
	Q_OBJECT

	public:
        explicit MetadataDateTimeDialog(const QString &file, QWidget *parent = 0);
		explicit MetadataDateTimeDialog(const QStringList &files, QWidget *parent = 0);
		~MetadataDateTimeDialog();

	private:
		bool loadData(const QString &file);
		bool saveData(const QString &file);

	public slots:
		virtual void accept();
		virtual void reject();

	private:
		Ui::MetadataDateTimeDialog *ui;
		QStringList m_fileList;

		QDateTime m_dateTime;
};

#endif
