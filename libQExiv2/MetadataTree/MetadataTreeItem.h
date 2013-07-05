#ifndef METADATA_ITEM_H
#define METADATA_ITEM_H

#include <QList>
#include <QVariant>

class MetadataTreeItem
{
	public:
		MetadataTreeItem(const QList<QVariant> &data, MetadataTreeItem *parent = 0);
		~MetadataTreeItem();

		void appendChild(MetadataTreeItem *child);

		MetadataTreeItem *child(int row);
		int childCount() const;
		int columnCount() const;
		QVariant data(int column) const;
		int row() const;
		MetadataTreeItem *parent();

	private:
		QList<MetadataTreeItem *> childItems;
		QList<QVariant> itemData;
		MetadataTreeItem *parentItem;
};
#endif
