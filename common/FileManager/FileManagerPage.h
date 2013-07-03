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

		QStringList fileSelection() const;
		QString currentPath() const;
		QString currentFile() const;
		QString currentFilePath() const;
		void previousFile();
		void nextFile();
		void updateInfo();

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

		void setFileManager(const QString &);

	private:
		Ui::FileManagerPage *ui;
		FileManager *m_fs1;
		FileManager *m_fs2;

		FileManager *m_lastActive;
};

#endif
