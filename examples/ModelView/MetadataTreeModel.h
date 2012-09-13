#ifndef METADATA_TREE_MODEL_H
#define METADATA_TREE_MODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

#if 0
class QExiv2;	// nascondere la struct exifData
#else
#include "QExiv2.h"
#endif

class MetadataTreeItem;

class MetadataTreeModel : public QAbstractItemModel
{
	Q_OBJECT

	public:
		MetadataTreeModel(QExiv2 *data, QObject *parent = 0);
		~MetadataTreeModel();

		QVariant data(const QModelIndex &index, int role) const;
		Qt::ItemFlags flags(const QModelIndex &index) const;
		QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
		QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
		QModelIndex parent(const QModelIndex &index) const;
		int rowCount(const QModelIndex &parent = QModelIndex()) const;
		int columnCount(const QModelIndex &parent = QModelIndex()) const;

	private:
		void setupModelData(MetadataTreeItem *parent);
		void addNode(QHash<QString, MetadataTreeItem *> &map, const exifData &data, MetadataTreeItem *parent);

		MetadataTreeItem *addFamilyNode(const exifData &data, MetadataTreeItem *parent);

	private:
		QExiv2 *m_metadata;

		MetadataTreeItem *rootItem;
};

#endif
