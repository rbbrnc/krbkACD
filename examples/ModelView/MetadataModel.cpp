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
	setupModelData(data, rootItem);
}

MetadataModel::~MetadataModel()
{
	delete rootItem;
}

int MetadataModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid()) {
		return static_cast<MetadataItem*>(parent.internalPointer())->columnCount();
	} else {
		return rootItem->columnCount();
	}
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

MetadataItem *MetadataModel::addFamilyNode(const exifData &data, MetadataItem *parent)
{
	QList<QVariant> columnData;
	columnData << data.family << "" << "" << "" << "" << "" << "";
	MetadataItem *node = new MetadataItem(columnData, parent);
	parent->appendChild(node);
	return node;
}

void MetadataModel::addNode(QHash<QString, MetadataItem *> &map, const exifData &data, MetadataItem *parent)
{
	QList<QVariant> columnData;
	MetadataItem *groupItem;

	//qDebug() << data.key;
	if (!map.contains(data.group)) {
		columnData << "" << data.group << "" << "" << "" << "" << "";
		groupItem = new MetadataItem(columnData, parent);
		parent->appendChild(groupItem);
		map[data.group] = groupItem;
	} else {
		groupItem = map.value(data.group);
	}

	columnData.clear();
	columnData << ""; //columnData << data.family;
	columnData << ""; //columnData << data.group;
	columnData << data.tagName;
	columnData << data.value;
	columnData << "0x" + QString::number(data.tag, 16);
	columnData << data.typeName;
	columnData << QString::number(data.count, 10);
	MetadataItem *item =  new MetadataItem(columnData, groupItem);
	groupItem->appendChild(item);
}

void MetadataModel::setupModelData(const QExiv2 *data, MetadataItem *parent)
{
	QList<exifData> exifData;
	MetadataItem *family;

	if (data->hasExif()) {
		exifData = data->exifDataList();
		if (exifData.size() > 0) {
			QHash<QString, MetadataItem *> groupMap;
			family = addFamilyNode(exifData.at(0), parent);
			for (int i = 0; i < exifData.size(); i++) {
				addNode(groupMap, exifData.at(i), family);
			}
		}
	}

	if (data->hasIptc()) {
		exifData = data->iptcDataList();
		if (exifData.size() > 0) {
			QHash<QString, MetadataItem *> groupMap;
			family = addFamilyNode(exifData.at(0), parent);
			for (int i = 0; i < exifData.size(); i++) {
				addNode(groupMap, exifData.at(i), family);
			}
		}
	}

	if (data->hasXmp()) {
		exifData = data->xmpDataList();
		if (exifData.size() > 0) {
			QHash<QString, MetadataItem *> groupMap;
			family = addFamilyNode(exifData.at(0), parent);
			for (int i = 0; i < exifData.size(); i++) {
				addNode(groupMap, exifData.at(i), family);
			}
		}
	}
}
