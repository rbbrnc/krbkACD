#ifndef EXIV2_VIEW_H
#define EXIV2_VIEW_H

#include <QWidget>
#include <QModelIndex>

#include <exiv2/exiv2.hpp>

#include <QTreeView>
#include <QStandardItemModel>
#include <QItemSelectionModel>

#include "file_data.h"
#include "QExiv2.h"
#include "PTag.h"

class Exiv2View : public QTreeView
{
	Q_OBJECT

	public:
		explicit Exiv2View(QWidget *parent = 0);
		~Exiv2View();

		void readMetadata(const QString file);
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

		void setMetadata(QStandardItem *rootItem, QString key, QString tag, QString typeName, QString count, QString value);

                QExiv2      m_metadata;
};

#endif
