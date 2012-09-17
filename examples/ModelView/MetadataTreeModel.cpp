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

#include "QExiv2.h"

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

	m_rootItem = new MetadataTreeItem(rootData);
	setupModelData(m_rootItem);
}

MetadataTreeModel::~MetadataTreeModel()
{
	delete m_rootItem;
}

int MetadataTreeModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid()) {
		return static_cast<MetadataTreeItem*>(parent.internalPointer())->columnCount();
	}

	return m_rootItem->columnCount();
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

	// Use Qt::WhatsThisRole for 'match' key data.
	// return 7th column (key)
	if (role == Qt::WhatsThisRole) {
		MetadataTreeItem *item = static_cast<MetadataTreeItem*>(index.internalPointer());
		return item->data(7);
	}

	return QVariant();
}

Qt::ItemFlags MetadataTreeModel::flags(const QModelIndex &index) const
{
	if (!index.isValid()) {
		return 0;
	}

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant MetadataTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
		return m_rootItem->data(section);
	}

	return QVariant();
}

QModelIndex MetadataTreeModel::index(int row, int column, const QModelIndex &parent) const
{
	if (!hasIndex(row, column, parent)) {
		return QModelIndex();
	}

	MetadataTreeItem *parentItem;

	if (!parent.isValid()) {
		parentItem = m_rootItem;
	} else {
		parentItem = static_cast<MetadataTreeItem*>(parent.internalPointer());
	}

	MetadataTreeItem *childItem = parentItem->child(row);
	if (childItem) {
		return createIndex(row, column, childItem);
	}

	return QModelIndex();
}

QModelIndex MetadataTreeModel::parent(const QModelIndex &index) const
{
	if (!index.isValid()) {
		return QModelIndex();
	}

	MetadataTreeItem *childItem = static_cast<MetadataTreeItem*>(index.internalPointer());
	MetadataTreeItem *parentItem = childItem->parent();

	if (parentItem == m_rootItem) {
		return QModelIndex();
	}

	return createIndex(parentItem->row(), 0, parentItem);
}

int MetadataTreeModel::rowCount(const QModelIndex &parent) const
{
	MetadataTreeItem *parentItem;
	if (parent.column() > 0) {
		return 0;
	}

	if (!parent.isValid()) {
		parentItem = m_rootItem;
	} else {
		parentItem = static_cast<MetadataTreeItem*>(parent.internalPointer());
	}

	return parentItem->childCount();
}

void MetadataTreeModel::addNode(QHash<QString, MetadataTreeItem *> &map,
				QList<QVariant> &columnData,
				MetadataTreeItem *parent)
{
	// Search for family Node
	QModelIndexList fNodes = match(index(0, 0), Qt::DisplayRole, columnData[0]);

	MetadataTreeItem *familyNode;
	if (fNodes.isEmpty()) {
		// Add a new family node
		QList<QVariant> familyData;
		familyData << columnData[0] << "" << "" << "" << "" << "" << "" << "";
		familyNode = new MetadataTreeItem(familyData, parent);
		parent->appendChild(familyNode);
	} else {
		familyNode = static_cast<MetadataTreeItem*>(fNodes.at(0).internalPointer());
	}

	MetadataTreeItem *groupNode;
	const QString  group  = columnData[1].toString();

	if (!map.contains(group)) {
		// Add a new group node to the model and to the map of groups.
		QList<QVariant> groupData;
		groupData << "" << group << "" << "" << "" << "" << "" << "";
		groupNode = new MetadataTreeItem(groupData, familyNode);
		familyNode->appendChild(groupNode);
		map[group] = groupNode;
	} else {
		groupNode = map.value(group);
	}

	// Clear 'family' and 'group' from leaf nodes
	columnData[0] = QVariant();
	columnData[1] = QVariant();
	MetadataTreeItem *item = new MetadataTreeItem(columnData, groupNode);
	groupNode->appendChild(item);
}

// Fill model with data
void MetadataTreeModel::setupModelData(MetadataTreeItem *parent)
{
	QList<exifData> data;

	if (m_metadata->hasExif()) {
		data = m_metadata->exifDataList();
	}

	if (m_metadata->hasIptc()) {
		data += m_metadata->iptcDataList();
	}

	if (m_metadata->hasXmp()) {
		data += m_metadata->xmpDataList();
	}

	if (data.size() > 0) {
		QList<QVariant> columnData;
		columnData << "" << "" << "" << "" << "" << "" << "" << "";
		QHash<QString, MetadataTreeItem *> groupMap;
		for (int i = 0; i < data.size(); i++) {
			columnData[0] = data.at(i).family;
			columnData[1] = data.at(i).group;
			columnData[2] = data.at(i).tagName;
			columnData[3] = data.at(i).value;
			columnData[4] = "0x" + QString::number(data.at(i).tag, 16);
			columnData[5] = data.at(i).typeName;
			columnData[6] = QString::number(data.at(i).count, 10);
			columnData[7] = data.at(i).key;

			addNode(groupMap, columnData, parent);
		}
	}

	// Image comment
	if (m_metadata->hasComment()) {
		QList<QVariant> columnData;
		columnData << "Comment" << "" << "" << m_metadata->imgComment() << "" << "" << "" << "";
		parent->appendChild(new MetadataTreeItem(columnData, parent));
	}

#if 0
	// Match example
	QModelIndexList Items = this->match(
		this->index(0, 0),
		Qt::WhatsThisRole,
		QVariant("Iptc.Application2.Keywords"),
		-1, // Match all items
		Qt::MatchRecursive); // look *

	qDebug()<< "MATCH=" << Items.size();
	for (int i = 0; i < Items.size(); ++i) {
		MetadataTreeItem *item = static_cast<MetadataTreeItem*>(Items.at(i).internalPointer());
		qDebug() << item->data(3);
	}
#endif
}
