#ifndef FILE_MANAGER_PAGE_H
#define FILE_MANAGER_PAGE_H

#include <QWidget>

namespace Ui {
	class FileManagerPage;
}

class FileManager;
class FileManagerPage : public QWidget
{
	Q_OBJECT

	public:
		explicit FileManagerPage(QWidget *parent = 0);
		~FileManagerPage();

		void setActiveFileManager();
		QString currentPath(bool useLastActiveFm = 0) const;
		QString currentFile(bool useLastActiveFm = 0) const;
		void previousFile(bool useLastActiveFm = 0);
		void nextFile(bool useLastActiveFm = 0);

	private:
		FileManager *activeFileManager(bool useLastActiveFm = 0) const;

	private slots:
		void showSecondFs(bool checked);

		void showIcons(bool show);
		void showHidden(bool show);
		void showInfo(bool show);

		void mkDir();
		void deleteFiles();
		void renameFiles();
		void copyFiles();
		void moveFiles();

	private:
		Ui::FileManagerPage *ui;
		FileManager *m_fs1;
		FileManager *m_fs2;

		FileManager *m_lastActive;
};

#endif
