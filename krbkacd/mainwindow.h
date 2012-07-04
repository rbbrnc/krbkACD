#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QMainWindow>

#include "fullscreen_page.h"
#include "browser_page.h"
#include "work_page.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

	signals:
		void showMetadata(bool);
		void showPreview(bool);
		void showSecondBrowser(bool);

	private slots:
		void fullScreen();
		void showWorkPage();

	private:
		Ui::MainWindow *ui;

		BrowserPage    *browserPage;
		FullScreenPage *fullScreenPage;
		WorkPage	*workPage;
};

#endif // MAINWINDOW_H
