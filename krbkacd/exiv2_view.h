#ifndef EXIV2_VIEW_H
#define EXIV2_VIEW_H

#include <QWidget>
#include <QModelIndex>

#include <exiv2/exiv2.hpp>

#include <QTreeView>
#include <QStandardItemModel>
#include <QItemSelectionModel>

class Exiv2View : public QTreeView
{
	Q_OBJECT

	public:
		explicit Exiv2View(QWidget *parent = 0);
		~Exiv2View();

		void readMetadata(const QString file);

	public slots:
		void debug();

	private:
		QStandardItemModel *standardModel;
		QStandardItem *exifItem;
		QStandardItem *iptcItem;
		QStandardItem *xmpItem;

		void setMetadata(QStandardItem *rootItem, QString key, QString tag, QString typeName, QString count, QString value);
};

#endif
