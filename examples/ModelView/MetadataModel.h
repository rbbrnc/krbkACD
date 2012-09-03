#ifndef METADATA_MODEL_H
#define METADATA_MODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

#include "QExiv2.h"

class MetadataItem;

class MetadataModel : public QAbstractItemModel
{
	Q_OBJECT

	public:
		MetadataModel(const QExiv2 *data, QObject *parent = 0);
		~MetadataModel();

		QVariant data(const QModelIndex &index, int role) const;
		Qt::ItemFlags flags(const QModelIndex &index) const;
		QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
		QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
		QModelIndex parent(const QModelIndex &index) const;
		int rowCount(const QModelIndex &parent = QModelIndex()) const;
		int columnCount(const QModelIndex &parent = QModelIndex()) const;

	private:
		void setupModelData(const QExiv2 *data, MetadataItem *parent);
		void addNode(const QString name, const QList<exifData> &data, MetadataItem *parent);

	private:
		MetadataItem *rootItem;
};

#endif
