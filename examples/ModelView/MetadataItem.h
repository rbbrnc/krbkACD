#ifndef METADATA_ITEM_H
#define METADATA_ITEM_H

#include <QList>
#include <QVariant>

class MetadataItem
{
	public:
		MetadataItem(const QList<QVariant> &data, MetadataItem *parent = 0);
		~MetadataItem();

		void appendChild(MetadataItem *child);

		MetadataItem *child(int row);
		int childCount() const;
		int columnCount() const;
		QVariant data(int column) const;
		int row() const;
		MetadataItem *parent();

	private:
		QList<MetadataItem*> childItems;
		QList<QVariant> itemData;
		MetadataItem *parentItem;
};
#endif
