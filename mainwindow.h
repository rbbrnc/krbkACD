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
		void updatePageData(int page, const QString &file);

	private slots:
		void onQuit();
		void onCurrentPathChanged();
		void onCurrentFileChanged();
		void onFullScreen(bool enable);

		void on_previewCheckBox_stateChanged(int state);

		void onChangePage(bool checked);

		void onIconMode(bool enable);
		void onShowHiddenFiles(bool enable);
		void onCopyFiles();
		void onMoveFiles();
		void onDeleteFiles();
		void onRenameFiles();
		void onMkDir();

		void onEditLocationsMetadata();
		void onEditSocialMetadata();
		void onEditMetadata();

	private:
		Ui::MainWindow *ui;

		FileManager *m_fm;
		FileManager *m_secondFm;

		QPixmap m_preview;

		QActionGroup *m_pageGroup;
};

#endif // MAINWINDOW_H
