/*
    Metadata Tree Item

    A container for items of data supplied by the Metadata Tree Model.
*/

#include <QStringList>

#include "MetadataTreeItem.h"

MetadataTreeItem::MetadataTreeItem(const QList<QVariant> &data, MetadataTreeItem *parent)
{
	parentItem = parent;
	itemData = data;
}

MetadataTreeItem::~MetadataTreeItem()
{
	qDeleteAll(childItems);
}

void MetadataTreeItem::appendChild(MetadataTreeItem *item)
{
	childItems.append(item);
}

MetadataTreeItem *MetadataTreeItem::child(int row)
{
	return childItems.value(row);
}

int MetadataTreeItem::childCount() const
{
	return childItems.count();
}

int MetadataTreeItem::columnCount() const
{
	return itemData.count();
}

QVariant MetadataTreeItem::data(int column) const
{
	return itemData.value(column);
}

MetadataTreeItem *MetadataTreeItem::parent()
{
	return parentItem;
}

int MetadataTreeItem::row() const
{
	if (parentItem) {
		return parentItem->childItems.indexOf(const_cast<MetadataTreeItem*>(this));
	}

	return 0;
}
