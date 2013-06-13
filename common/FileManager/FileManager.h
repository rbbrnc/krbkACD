#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QWidget>
#include <QFileSystemModel>
#include <QDir>
#include <QItemSelectionModel>
#include <QKeyEvent>

namespace Ui {
	class FileManager;
}

class FSManager;
class FileManager : public QWidget
{
	Q_OBJECT

	public:
		FileManager(QWidget *parent = 0);
		~FileManager();

		QString currentPath() const;
		QString currentFile() const;
		bool isActive() const;

		void updateInfo();

	private:
		void updateGeneralInfo(const QModelIndex &index);
		void updatePreview(const QModelIndex &index);

	private slots:

		void fileSelect(const QModelIndex &current, const QModelIndex &previous);
		void handleItemActivation(QModelIndex index);

		void zoomPreview(int , int);

	public slots:
		void previous();
		void next();

		void mkDir();
		void remove();
	        void rename();
	        void copy(const QString &destPath);
	        void move(const QString &destPath);

		void iconMode(bool enable);
		void showHidden(bool show);
		void showInfo(bool show);

	signals:
		void currentChanged(const QString &newFile);

	protected:
		void keyPressEvent(QKeyEvent *event);
		void keyReleaseEvent(QKeyEvent *event);

	private:
		Ui::FileManager *ui;

		QFileSystemModel *m_model;
		QModelIndex m_currentIndex;

		QString m_currentFileName;
		QDir    m_currentDir;

		QModelIndexList m_selection;
		QItemSelectionModel *m_selectionModel;

		QPixmap m_preview;
};
#endif
