/*
    MetadataItem.cpp

    A container for items of data supplied by the Metadata tree model.
*/

#include <QStringList>

#include "MetadataItem.h"

MetadataItem::MetadataItem(const QList<QVariant> &data, MetadataItem *parent)
{
	parentItem = parent;
	itemData = data;
}

MetadataItem::~MetadataItem()
{
	qDeleteAll(childItems);
}

void MetadataItem::appendChild(MetadataItem *item)
{
	childItems.append(item);
}

MetadataItem *MetadataItem::child(int row)
{
	return childItems.value(row);
}

int MetadataItem::childCount() const
{
	return childItems.count();
}

int MetadataItem::columnCount() const
{
	return itemData.count();
}

QVariant MetadataItem::data(int column) const
{
	return itemData.value(column);
}

MetadataItem *MetadataItem::parent()
{
	return parentItem;
}

int MetadataItem::row() const
{
	if (parentItem) {
		return parentItem->childItems.indexOf(const_cast<MetadataItem*>(this));
	}

	return 0;
}
