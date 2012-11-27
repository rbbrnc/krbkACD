#ifndef REGION_GRAPHICS_ITEM_H
#define REGION_GRAPHICS_ITEM_H

#include <QGraphicsRectItem>

class RegionGraphicsItem : public QGraphicsRectItem
{
	public:
		RegionGraphicsItem(QGraphicsItem *parent = 0);
		RegionGraphicsItem(const QRectF &rect, QGraphicsItem *parent = 0);
		RegionGraphicsItem(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent = 0);
		~RegionGraphicsItem();

		QString name() const;
		void setName(const QString &name);

		QString description() const;
		void setDescription(const QString &desc);

	protected:
		virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
		virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
		virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
		virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
		virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

		virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

	private:
		QString m_name;
		QString m_description;
};

#endif
