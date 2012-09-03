/*
    treemodel.cpp

    Provides a simple tree model to show how to create and use hierarchical
    models.
*/

#include <QtGui>
#include <QDebug>

#include "MetadataItem.h"
#include "MetadataModel.h"

MetadataModel::MetadataModel(const QExiv2 *data, QObject *parent)
    : QAbstractItemModel(parent)
{
	QList<QVariant> rootData;
	rootData << "Family"
		 << "Group"
		 << "Tag"
		 << "Value"
		 << "Id"
		 << "Type"
		 << "Count";

	rootItem = new MetadataItem(rootData);

	if (data->hasExif()) {
		addNode("EXIF", data->exifDataList(), rootItem);
	}
	if (data->hasIptc()) {
		addNode("IPTC", data->iptcDataList(), rootItem);
	}
	if (data->hasXmp()) {
		addNode("XMP", data->xmpDataList(), rootItem);
	}

//	setupModelData(data, rootItem);
}

MetadataModel::~MetadataModel()
{
	delete rootItem;
}

int MetadataModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return static_cast<MetadataItem*>(parent.internalPointer())->columnCount();
	else
		return rootItem->columnCount();
}

QVariant MetadataModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();

	MetadataItem *item = static_cast<MetadataItem*>(index.internalPointer());

	return item->data(index.column());
}

Qt::ItemFlags MetadataModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return 0;

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant MetadataModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return rootItem->data(section);

	return QVariant();
}

QModelIndex MetadataModel::index(int row, int column, const QModelIndex &parent) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	MetadataItem *parentItem;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<MetadataItem*>(parent.internalPointer());

	MetadataItem *childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

QModelIndex MetadataModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	MetadataItem *childItem = static_cast<MetadataItem*>(index.internalPointer());
	MetadataItem *parentItem = childItem->parent();

	if (parentItem == rootItem)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}

int MetadataModel::rowCount(const QModelIndex &parent) const
{
	MetadataItem *parentItem;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<MetadataItem*>(parent.internalPointer());

	return parentItem->childCount();
}

void MetadataModel::addNode(const QString name, const QList<exifData> &data, MetadataItem *parent)
{
	QList<QVariant> c;
	c << name << "" << "" << "" << "" << "" << "";
	MetadataItem *node = new MetadataItem(c, parent);
	parent->appendChild(node);

	QList<MetadataItem *> parents;
	parents << node;

	for (int i = 0; i < data.size(); i++) {
		struct exifData d = data.at(i);
		QList<QVariant> columnData;
		columnData << d.family;
		columnData << d.group;
		columnData << d.tagName;
//		columnData << d.key;
		columnData << d.value;
		columnData << "0x" + QString::number(d.tag, 16);
		columnData << d.typeName;
		columnData << QString::number(d.count, 10);
		parents.last()->appendChild(new MetadataItem(columnData, parents.last()));
	}
}

void MetadataModel::setupModelData(const QExiv2 *data, MetadataItem *parent)
{
	if (data->hasExif()) {
		addNode("Exif", data->exifDataList(), parent);
	}
	if (data->hasIptc()) {
		addNode("Iptc", data->iptcDataList(), parent);
	}
	if (data->hasXmp()) {
		addNode("Xmp", data->xmpDataList(), parent);
	}
}
