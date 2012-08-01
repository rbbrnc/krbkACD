#ifndef WORK_PAGE_H
#define WORK_PAGE_H

#include <QWidget>
#include <QFileSystemModel>

#include "file_data.h"
#include "histogram.h"

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
		void fillFileList(const QString path, bool recursive);

		void debugDupMap() const;
		void debugHistogramMap() const;
		void debugFileList() const;

		// Compare Methods
		void compareFileMd5();
		void compareMetadata();
		void compareByteToByte();
		void compareImage();
		void compareHistogram();

	signals:
		void changePage(int);

	private slots:
		void on_insertDir_clicked();
		void on_removeDir_clicked();
		void on_workButton_clicked();
		void on_resultsButton_clicked();

		void dirSelectionChanged(const QModelIndex &index);

		void on_clearDir_clicked();
		void on_compareType_currentIndexChanged(int index);

	        void on_tresholdHistogramDiffSlider_valueChanged(int value);

	private:
		Ui::WorkPage *ui;

		QFileSystemModel m_dirModel;
		QString		 m_currentDirSelection;
		QString		 m_currentWorkingDirSelection;

		bool m_mimeFilter;

		// map of duplicate file [Key = md5, Value = File Path]
		QMap<QString, QStringList> m_dupMap;
		QMap<QString, ColorHistogram> m_histogramMap;
		QList<FileData> m_fileList;
};

#endif // WORK_PAGE_H
