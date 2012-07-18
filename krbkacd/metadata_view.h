#ifndef METADATA_VIEW_H
#define METADATA_VIEW_H

#include <QWidget>
#include <QModelIndex>

#include <exiv2/exiv2.hpp>

#include <QTreeView>
#include <QStandardItemModel>
#include <QItemSelectionModel>

#include "file_data.h"
#include "QExiv2.h"
#include "PTag.h"

class MetadataView : public QTreeView
{
	Q_OBJECT

	public:
		explicit MetadataView(QWidget *parent = 0);
		~MetadataView();

		void readMetadata();
		void clearMetadata();

		void setFileData(FileData fdata);

	public slots:
		void debug();

	private:
		QStandardItemModel *standardModel;
		QStandardItem *exifItem;
		QStandardItem *iptcItem;
		QStandardItem *xmpItem;
		QStandardItem *commentItem;

		void setMetadata(QStandardItem *rootItem, struct exifData *data);

                QExiv2      m_metadata;
};

#endif
