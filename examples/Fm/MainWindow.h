#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


class FileManager;

namespace Ui {
	class MainWindow;
}

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

		void fullScreen(bool enable);
		void showImageRegions(bool enable);

	private:
		void loadImage(const QString &fileName, bool loadMetadata);

	private:
		Ui::MainWindow *ui;
		int m_currentFileManager;

		FileManager *m_fm;
};

#endif
