#ifndef METADATATREEVIEWPAGE_H
#define METADATATREEVIEWPAGE_H

#include <QWidget>

#include "MetadataTreeModel.h"
#include "QExiv2.h"

namespace Ui {
	class MetadataTreeViewPage;
}


class MetadataTreeViewPage : public QWidget
{
	Q_OBJECT

	public:
		explicit MetadataTreeViewPage(QWidget *parent = 0);
		~MetadataTreeViewPage();

		void setFile(const QString &file);

	private slots:
		void next();
		void previous();

	signals:
		void requestNextFile();
		void requestPreviousFile();

	private:
		Ui::MetadataTreeViewPage *ui;

		MetadataTreeModel *m_model;
};

#endif
