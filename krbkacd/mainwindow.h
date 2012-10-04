#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QMainWindow>

// UI Pages
#include "browser_page.h"
#include "work_page.h"
//#include "photo_widget.h"
//#include "metadata_editor_page.h"
#include "duplicate_page.h"

class ImageViewManager;

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

	public slots:
		void changePageSlot(int);
		void updateStatusBar();
		void debug();

	private slots:
		void fullScreen();
		void showWorkPage();
		void showMetadataEditorPage();

		void renameFile();
		void deleteFile();

	private:
		Ui::MainWindow *ui;

		int m_prevPageIndex;
		BrowserPage *m_browserPage;
		WorkPage    *m_workPage;
//		PhotoWidget *m_photoPage;
		ImageViewManager *m_photoPage;
//		MetadataEditorPage *m_metadataEditorPage;
		DuplicatePage *m_duplicatePage;
};

#endif // MAINWINDOW_H
