#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
	class MainWindow;
}

class FileManager;
class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

	private:
		FileManager *activeFileManager() const;

	private slots:
		void showSecondFs(bool checked);
		void previousFile();
		void nextFile();
		void mkDir();
		void deleteFiles();
		void renameFiles();
		void copyFiles();
		void moveFiles();

	private:
		Ui::MainWindow *ui;
		FileManager *m_fs1;
		FileManager *m_fs2;
};

#endif // MAINWINDOW_H
