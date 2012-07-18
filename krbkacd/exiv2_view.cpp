#include "exiv2_view.h"

#include <QDebug>

Exiv2View::Exiv2View(QWidget *parent) :
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

void Exiv2View::setMetadata(QStandardItem *rootItem, QString key, QString tag, QString typeName, QString count, QString value)
{
        QList<QStandardItem *> items;
	QStandardItem *item =  new QStandardItem();
	items << item;

        QStandardItem *keyItem =  new QStandardItem("Key");
        keyItem->setText(key);
        keyItem->setEditable(false);
        items << keyItem;

        QStandardItem *tagItem =  new QStandardItem("Tag");
        tagItem->setText(tag);
        tagItem->setEditable(false);
        items << tagItem;

        QStandardItem *typeNameItem =  new QStandardItem("Type Name");
        typeNameItem->setText(typeName);
        typeNameItem->setEditable(false);
        items << typeNameItem;

        QStandardItem *countItem =  new QStandardItem("Count");
        countItem->setText(count);
        countItem->setEditable(false);
        items << countItem;

        QStandardItem *valueItem =  new QStandardItem("Value");
        valueItem->setText(value);
        valueItem->setEditable(true);
        items << valueItem;

        rootItem->appendRow(items);

	// DEBUG
	//qDebug() << key << " " << tag << " " << typeName << "" << count << " " << value;
}

Exiv2View::~Exiv2View()
{
}

void Exiv2View::debug()
{
}

void Exiv2View::setFileData(FileData fdata)
{
	// Clear previous metadata
	clearMetadata();

	m_metadata = fdata.metadata();

#if 0
	QList<PTag> tl = m_metadata.xmpPTags();
	for (int i = 0; i < tl.size(); i++)
		tl.at(i).debug();
#endif
}

// Clear metadata if present!
void Exiv2View::clearMetadata()
{
	if (standardModel->rowCount() > 0) {
		if (exifItem->rowCount() > 0) {
			//qDebug() << __func__ << "EXIF rowCount=" << exifItem->rowCount();
			exifItem->removeRows(0, exifItem->rowCount());
		}
		if (iptcItem->rowCount() > 0) {
			//qDebug() << __func__ << "IPTC rowCount=" << iptcItem->rowCount();
			iptcItem->removeRows(0, iptcItem->rowCount());
		}
		if (xmpItem->rowCount() > 0) {
			//qDebug() << __func__ << "XMP rowCount=" << xmpItem->rowCount();
			xmpItem->removeRows(0, xmpItem->rowCount());
		}
		if (commentItem->rowCount() > 0) {
			//qDebug() << __func__ << "Comment rowCount=" << commentItem->rowCount();
			commentItem->removeRows(0, commentItem->rowCount());
		}
	}
}

void Exiv2View::readMetadata(const QString file)
{
	if (m_metadata.hasExif()) {
		//qDebug() << "Found Exif data";
		QList<exifData> ed = m_metadata.exifDataList();
		for (int i=0; i < ed.size(); i++) {
			//qDebug() << ed.at(i).key;
			struct exifData data = ed.at(i);
			setMetadata(exifItem, data.key,
				    "0x" + QString::number(data.tag, 16),
				    data.typeName,
				    QString::number(data.count, 10),
				    data.value);
		}
	}

	if (m_metadata.hasIptc()) {
		//qDebug() << "Found Iptc data";
		QList<exifData> ed = m_metadata.iptcDataList();
		for (int i=0; i < ed.size(); i++) {
			//qDebug() << ed.at(i).key;
			struct exifData data = ed.at(i);
			setMetadata(iptcItem, data.key,
				    "0x" + QString::number(data.tag, 16),
				    data.typeName,
				    QString::number(data.count, 10),
				    data.value);
		}

	}

	if (m_metadata.hasXmp()) {
		//qDebug() << "Found XMP data";
		//qDebug() << m_metadata.xmpPacket();
		QList<exifData> ed = m_metadata.xmpDataList();
		for (int i=0; i < ed.size(); i++) {
			//qDebug() << ed.at(i).key;
			struct exifData data = ed.at(i);
			setMetadata(xmpItem, data.key,
				    "0x" + QString::number(data.tag, 16),
				    data.typeName,
				    QString::number(data.count, 10),
				    data.value);
		}
	}

	// Add image comment to the view
	if (m_metadata.hasComment()) {
		//qDebug() << "Img Comment:" << m_metadata.imgComment();
		QStandardItem *item =  new QStandardItem();
	        item->setText(m_metadata.imgComment());
	        item->setEditable(false);
		commentItem->appendRow(item);
	}

}
