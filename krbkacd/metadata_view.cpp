#include "metadata_view.h"

#include <QDebug>

MetadataView::MetadataView(QWidget *parent) :
	QTreeView(parent)
{
	standardModel = new QStandardItemModel;

	QStringList headerLabels;
	headerLabels << "Metadata";
	headerLabels << "Key";
	headerLabels << "Tag";
	headerLabels << "Type Name";
	headerLabels << "Count";
	headerLabels << "Value";

	standardModel->setHorizontalHeaderLabels(headerLabels);

	QStandardItem *rootNode = standardModel->invisibleRootItem();

	// Defining root items
	exifItem = new QStandardItem("EXIF");
	iptcItem = new QStandardItem("IPTC");
	xmpItem  = new QStandardItem("XMP");
	commentItem = new QStandardItem("Comment");

	// Building up the hierarchy
	rootNode->appendRow(exifItem);
	rootNode->appendRow(iptcItem);
	rootNode->appendRow(xmpItem);
	rootNode->appendRow(commentItem);

	// Register the model
	setModel(standardModel);
	//expandAll();
}

void MetadataView::setMetadata(QStandardItem *rootItem, struct exifData *data)
{
        QList<QStandardItem *> items;
	QStandardItem *item =  new QStandardItem();
	items << item;

        QStandardItem *keyItem =  new QStandardItem("Key");
        keyItem->setText(data->key);
        keyItem->setEditable(false);
        items << keyItem;

        QStandardItem *tagItem =  new QStandardItem("Tag");
        tagItem->setText("0x" + QString::number(data->tag, 16));
        tagItem->setEditable(false);
        items << tagItem;

        QStandardItem *typeNameItem =  new QStandardItem("Type Name");
        typeNameItem->setText(data->typeName);
        typeNameItem->setEditable(false);
        items << typeNameItem;

        QStandardItem *countItem =  new QStandardItem("Count");
        countItem->setText(QString::number(data->count, 10));
        countItem->setEditable(false);
        items << countItem;

        QStandardItem *valueItem =  new QStandardItem("Value");
        valueItem->setText(data->value);
        valueItem->setEditable(true);
        items << valueItem;

        rootItem->appendRow(items);

	// DEBUG
	//qDebug() << key << " " << tag << " " << typeName << "" << count << " " << value;
}

MetadataView::~MetadataView()
{
}

void MetadataView::debug()
{
}

void MetadataView::setFileData(FileData fdata)
{
	// Clear previous metadata
	clearMetadata();
	m_metadata = fdata.metadata();
}

// Clear metadata if present!
void MetadataView::clearMetadata()
{
	if (standardModel->rowCount() > 0) {
		if (exifItem->rowCount() > 0) {
			exifItem->removeRows(0, exifItem->rowCount());
		}
		if (iptcItem->rowCount() > 0) {
			iptcItem->removeRows(0, iptcItem->rowCount());
		}
		if (xmpItem->rowCount() > 0) {
			xmpItem->removeRows(0, xmpItem->rowCount());
		}
		if (commentItem->rowCount() > 0) {
			commentItem->removeRows(0, commentItem->rowCount());
		}
	}
}

void MetadataView::readMetadata()
{
	if (m_metadata.hasExif()) {
		QList<exifData> ed = m_metadata.exifDataList();
		for (int i=0; i < ed.size(); i++) {
			struct exifData data = ed.at(i);
			setMetadata(exifItem, &data);
		}
	}

	if (m_metadata.hasIptc()) {
		QList<exifData> ed = m_metadata.iptcDataList();
		for (int i=0; i < ed.size(); i++) {
			struct exifData data = ed.at(i);
			setMetadata(iptcItem, &data);
		}

	}

	if (m_metadata.hasXmp()) {
		QList<exifData> ed = m_metadata.xmpDataList();
		for (int i=0; i < ed.size(); i++) {
			struct exifData data = ed.at(i);
			setMetadata(xmpItem, &data);
		}
	}

	// Add image comment to the view
	if (m_metadata.hasComment()) {
		QStandardItem *item =  new QStandardItem();
	        item->setText(m_metadata.imgComment());
	        item->setEditable(false);
		commentItem->appendRow(item);
	}

}
