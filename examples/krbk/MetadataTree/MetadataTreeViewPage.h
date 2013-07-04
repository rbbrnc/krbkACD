#ifndef METADATA_TREE_VIEW_PAGE_H
#define METADATA_TREE_VIEW_PAGE_H

#include <QWidget>
#include "MetadataTreeModel.h"

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

	private:
		Ui::MetadataTreeViewPage *ui;

		MetadataTreeModel *m_model;
};
#endif
