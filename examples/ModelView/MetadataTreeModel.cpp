//
// Metadata Tree Model
//
//
// TODO:
//	Invece di usare le QString gestire i QVariant per i tipi di dati
//	Rendere il modello editabile;
//	Metodo save per salvare su file le modifiche (edit)
//	Gestire la gerarchia XmpBag, XmpSeq, XmpStruct ...
//

#include <QtGui>
#include <QDebug>

#include "MetadataTreeItem.h"
#include "MetadataTreeModel.h"

MetadataTreeModel::MetadataTreeModel(QExiv2 *data, QObject *parent)
	: QAbstractItemModel(parent),
	  m_metadata(data)
{
	QList<QVariant> rootData;
	rootData << "Family"
		 << "Group"
		 << "Tag"
		 << "Value"
		 << "Id"
		 << "Type"
		 << "Count"
		 << "key";

	rootItem = new MetadataTreeItem(rootData);
	setupModelData(rootItem);
}

MetadataTreeModel::~MetadataTreeModel()
{
	delete rootItem;
}

int MetadataTreeModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid()) {
		return static_cast<MetadataTreeItem*>(parent.internalPointer())->columnCount();
	} else {
		return rootItem->columnCount();
	}
}

QVariant MetadataTreeModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid()) {
		return QVariant();
	}

	if (role == Qt::DisplayRole) {
		MetadataTreeItem *item = static_cast<MetadataTreeItem*>(index.internalPointer());
		return item->data(index.column());
	}

	// Use Qt::WhatsThisRole for 'match' calls.
	// return 7th column (key)
	if (role == Qt::WhatsThisRole) {
		MetadataTreeItem *item = static_cast<MetadataTreeItem*>(index.internalPointer());
		return item->data(7);
	}


	return QVariant();
}

Qt::ItemFlags MetadataTreeModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return 0;

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant MetadataTreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return rootItem->data(section);

	return QVariant();
}

QModelIndex MetadataTreeModel::index(int row, int column, const QModelIndex &parent) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	MetadataTreeItem *parentItem;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<MetadataTreeItem*>(parent.internalPointer());

	MetadataTreeItem *childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

QModelIndex MetadataTreeModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	MetadataTreeItem *childItem = static_cast<MetadataTreeItem*>(index.internalPointer());
	MetadataTreeItem *parentItem = childItem->parent();

	if (parentItem == rootItem)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}

int MetadataTreeModel::rowCount(const QModelIndex &parent) const
{
	MetadataTreeItem *parentItem;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<MetadataTreeItem*>(parent.internalPointer());

	return parentItem->childCount();
}

MetadataTreeItem *MetadataTreeModel::addFamilyNode(const exifData &data, MetadataTreeItem *parent)
{
	QList<QVariant> columnData;
	columnData << data.family << "" << "" << "" << "" << "" << "" << "";
	MetadataTreeItem *node = new MetadataTreeItem(columnData, parent);
	parent->appendChild(node);
	return node;
}

void MetadataTreeModel::addNode(QHash<QString, MetadataTreeItem *> &map, const exifData &data, MetadataTreeItem *parent)
{
	QList<QVariant> columnData;
	MetadataTreeItem *groupItem;

	//qDebug() << data.key;
	if (!map.contains(data.group)) {
		columnData << "" << data.group << "" << "" << "" << "" << "" << "";
		groupItem = new MetadataTreeItem(columnData, parent);
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
	columnData << data.key;

	MetadataTreeItem *item =  new MetadataTreeItem(columnData, groupItem);
	groupItem->appendChild(item);

}

void MetadataTreeModel::setupModelData(MetadataTreeItem *parent)
{
	QList<exifData> exifData;
	MetadataTreeItem *family;

	if (m_metadata->hasExif()) {
		exifData = m_metadata->exifDataList();
		if (exifData.size() > 0) {
			QHash<QString, MetadataTreeItem *> groupMap;
			family = addFamilyNode(exifData.at(0), parent);
			for (int i = 0; i < exifData.size(); i++) {
				addNode(groupMap, exifData.at(i), family);
			}
		}
	}

	if (m_metadata->hasIptc()) {
		exifData = m_metadata->iptcDataList();
		if (exifData.size() > 0) {
			QHash<QString, MetadataTreeItem *> groupMap;
			family = addFamilyNode(exifData.at(0), parent);
			for (int i = 0; i < exifData.size(); i++) {
				addNode(groupMap, exifData.at(i), family);
			}
		}
	}

	if (m_metadata->hasXmp()) {
		exifData = m_metadata->xmpDataList();
		if (exifData.size() > 0) {
			QHash<QString, MetadataTreeItem *> groupMap;
			family = addFamilyNode(exifData.at(0), parent);
			for (int i = 0; i < exifData.size(); i++) {
				addNode(groupMap, exifData.at(i), family);
			}
		}
	}


	// Match example
	QModelIndexList Items = this->match(
		this->index(0, 0),
		Qt::WhatsThisRole,
		QVariant("Iptc.Application2.Keywords"),
		-1, // Match all items
		Qt::MatchRecursive); // look *

	qDebug()<< "MATCH=" << Items.size();
	if (Items.size() > 0) {
		MetadataTreeItem *item = static_cast<MetadataTreeItem*>(Items.at(0).internalPointer());
		qDebug() << item->data(3);

	}
}
