#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
	class MainWindow;
}

class FileManagerPage;
class ImageViewManager;
class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

	private slots:
		void showImage();
		void showMetadata();
		void nextFile();
		void prevFile();

	private:
		Ui::MainWindow *ui;

		FileManagerPage  *m_fmPage;
		int m_currentFileManager;

		ImageViewManager *m_ivPage;
		QWidget *m_mvPage;

};

#endif
