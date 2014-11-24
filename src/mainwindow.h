#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>

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

	private:
		void updatePreview(const QString &file);
		void updatePageData(int page);

		QStringList fileSelection() const;

	private slots:
		void onQuit();
		void onFullScreen(bool enable);

		void on_previewCheckBox_stateChanged(int state);

		void onChangePage(bool checked);

		void onCopyFiles();
		void onMoveFiles();

		void onEditLocationsMetadata();
		void onEditSocialMetadata();
		void onEditMetadata();
		void onEditDateTimeMetadata();

		void scalePreview(int, int);
		void showFmContextMenu(const QPoint &pos);
		void onOpenFile();

        void on_actionStart_Process_triggered();
        void on_actionPreferences_triggered();
        void on_actionAbout_triggered();
        void on_actionEditKeywords_triggered();

    private:
		Ui::MainWindow *ui;

        FileManager *m_fm1;
        FileManager *m_fm2;

		QPixmap m_preview;

		QActionGroup *m_pageGroup;
};

#endif // MAINWINDOW_H
