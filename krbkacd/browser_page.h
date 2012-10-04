#ifndef BROWSER_PAGE_H
#define BROWSER_PAGE_H

#include <QWidget>
#include <QFileSystemModel>
#include <QKeyEvent>

#include "file_data.h"

namespace Ui {
	class BrowserPage;
}

class MetadataTreeModel;

class BrowserPage : public QWidget
{
	Q_OBJECT

	public:
		explicit BrowserPage(QWidget *parent = 0);
		~BrowserPage();

		QPixmap currentPixmap();
		QString currentPath() const;
		FileData currentFileData() const;

		QStringList selectedFiles(QDir::Filter filters = QDir::Files) const;

	signals:
		void changePage(int);
		void currentChanged();

	public slots:
		void showHiddenFiles(bool show);
		void showPreview(bool show);
		void showMetadata(bool show);
		void showSecondBrowser(bool show);
		void showIcons(bool show);

		void copyFile();
		void moveFile();
		void deleteFile();
		void renameFile();
		void makeNewDirectory();


	private slots:
		void updateCurrentSelection(const QString file);
		void previewSplitterMoved(int pos, int index);

	protected:
		void keyPressEvent(QKeyEvent *e);
		void keyReleaseEvent(QKeyEvent *e);

	private:
		void updatePreview();

	private:
		Ui::BrowserPage *ui;
		FileData m_currentFileData;
		MetadataTreeModel *m_metadataModel;
};

#endif // BROWSER_PAGE_H
