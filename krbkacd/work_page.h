#ifndef WORK_PAGE_H
#define WORK_PAGE_H

#include <QWidget>
#include <QFileSystemModel>

#include "file_data.h"

namespace Ui {
	class WorkPage;
}

class WorkPage : public QWidget
{
	Q_OBJECT

	public:
		explicit WorkPage(QWidget *parent = 0);
		~WorkPage();

	private:
		void scanDir(const QString path, bool recursive);
		void compareFileMd5(FileData &fdata, QMap<QString, QStringList> &map);
		void compareMetadata(FileData &fdata, QMap<QString, QStringList> &map);

	private slots:
		void on_insertDir_clicked();
		void on_removeDir_clicked();
		void on_workButton_clicked();

		void dirSelectionChanged(const QModelIndex &index);

		void on_clearDir_clicked();

	private:
		Ui::WorkPage *ui;

		QFileSystemModel m_dirModel;
		QString		 m_currentDirSelection;
		QString		 m_currentWorkingDirSelection;

		bool m_mimeFilter;
};

#endif // WORK_PAGE_H
