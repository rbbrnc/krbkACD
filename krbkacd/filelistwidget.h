#ifndef FILELISTWIDGET_H
#define FILELISTWIDGET_H

#include <QListView>
#include <QFileSystemModel>
#include <QDir>

class FileListWidget : public QListView
{
	Q_OBJECT

	public:
		explicit FileListWidget(QWidget *parent = 0);

		QString getPath();

		// Return all directories from current selection
		//QStringList selectedDirs();

		QStringList selectedFiles();

	signals:
		void pathChanged(QString newPath);
		void selectionChanged(const QString newPath);

	public slots:
		void changePath(QString path);
		void actionSwitchMode(bool iconmode=true);
		void actionShowHidden(bool show=true);

		void actionDelete();
		void actionRename();
		void actionCopyFile(const QString destPath);
		void actionMoveFile(const QString destPath);
		void actionMkDir(const QString destPath);

	private slots:
		void handleItemActivation(QModelIndex index);
		void fileSelect(const QModelIndex &current, const QModelIndex &previous);

	private:
		QFileSystemModel *m_model;

		QDir m_currentDir;

		QModelIndexList      m_selection;
		QItemSelectionModel *m_selectionModel;
};

#endif // FILELISTWIDGET_H
