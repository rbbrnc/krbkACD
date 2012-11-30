#ifndef REGION_GRAPHICS_ITEM_H
#define REGION_GRAPHICS_ITEM_H

#include <QGraphicsRectItem>

class RegionGraphicsItem : public QObject, public QGraphicsRectItem
{
	Q_OBJECT

	public:
		RegionGraphicsItem(QGraphicsItem *parent = 0);
		RegionGraphicsItem(const QRectF &rect, QGraphicsItem *parent = 0);
		RegionGraphicsItem(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent = 0);
		~RegionGraphicsItem();

		QString name() const;
		void setName(const QString &name);

		QString description() const;
		void setDescription(const QString &desc);

		bool isNormalized() const;
		void setNormalized(bool normalized);

	protected:
		virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
		virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
		virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
		virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
		virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

		virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

	private:
		void init();

	signals:
		void removeRequest();
		void editRequest();

	private:
		unsigned int m_type;	// MP or MWG

		QString m_name;		// Region Name
		QString m_text;		// Region Description
		QRectF  m_region;	// Region rectangle
		bool    m_normalized;   // Rectangle is normalized [0, 1]
};

#endif
