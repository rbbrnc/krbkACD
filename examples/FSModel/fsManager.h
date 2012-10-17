#ifndef FS_MANAGER_H
#define FS_MANAGER_H

#include <QFileSystemModel>
#include <QDir>

class FSManager : public QObject
{
	Q_OBJECT

	private:
		FSManager();

		static FSManager *m_instance;

	public:
		static FSManager *instance();
		~FSManager();

		QFileSystemModel *model() const;
		QModelIndex currentIndex() const;

		QModelIndex firstRow() const;
		QModelIndex nextRow(const QModelIndex &current) const;
		QModelIndex nextRow(int current) const;
		QModelIndex previousRow(const QModelIndex &current) const;
		QModelIndex previousRow(int current) const;

	public slots:
		void changePath(const QString &path);
		void showHiddenFiles(bool show);

		bool removeFiles(QModelIndexList &list);

	private:
		QFileSystemModel *m_model;
		QModelIndex m_currentIndex;
		QString m_path;
};

#endif
