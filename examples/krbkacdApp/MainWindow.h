#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
	class MainWindow;
}

class FileManagerPage;
class ImageViewManager;
class MetadataTreeViewPage;

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
		void detectObjects();

		void fullScreen(bool enable);
		void showImageRegions(bool enable);

	private:
		void loadImage(const QString &fileName, bool loadMetadata);

	private:
		Ui::MainWindow *ui;

		FileManagerPage  *m_fmPage;
		int m_currentFileManager;

		ImageViewManager *m_ivPage;
		MetadataTreeViewPage *m_mvPage;
};

#endif
