#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <QFileSystemModel>
#include <QDir>
#include <QItemSelectionModel>
#include <QListView>

class FileManager : public QObject
{
	Q_OBJECT

	public:
		FileManager(QListView *listView, QWidget *parent = 0);
		~FileManager();

		QString currentPath() const;
		QString currentFile() const;
		QStringList fileSelection() const;

		bool isActive() const;

//	private:
//		void scrollToCurrent();

	private slots:

		void fileSelect(const QModelIndex &current, const QModelIndex &previous);
		void handleItemActivation(QModelIndex index);

	public slots:
		void previous();
		void next();

		void mkdir();
	        void deleteSelectedFiles();
	        void renameSelectedFiles();
	        void copy(const QString &destPath);
	        void move(const QString &destPath);

		void iconMode(bool enable);
		void showHidden(bool enable);

	signals:
		void currentChanged();
		void currentPathChanged();

	private:

		QListView *m_view;
		QWidget   *m_parent;

		QFileSystemModel *m_model;
		QModelIndex m_currentIndex;

		QString m_currentFileName;
		QDir    m_currentDir;

		QModelIndexList m_selection;
		QItemSelectionModel *m_selectionModel;
};
#endif
