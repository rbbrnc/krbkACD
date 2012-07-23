#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QMainWindow>

#include "browser_page.h"
#include "work_page.h"

#include "photo_widget.h"
#include "metadata_editor_page.h"

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
		void showMetadataEditorPage();

	private:
		Ui::MainWindow *ui;

		BrowserPage *browserPage;
		WorkPage    *workPage;
		PhotoWidget *m_photoPage;
		MetadataEditorPage *m_metadataEditorPage;
};

#endif // MAINWINDOW_H
