#ifndef REGION_GRAPHICS_ITEM_H
#define REGION_GRAPHICS_ITEM_H

#include <QGraphicsRectItem>

#include "XmpRegion.h"

class RegionGraphicsItem : public QObject, public QGraphicsRectItem
{
	Q_OBJECT

	public:
		RegionGraphicsItem(const XmpRegion &region, QGraphicsItem *parent = 0);
		~RegionGraphicsItem();

		QString name() const;
		void setName(const QString &name);

		QString description() const;
		void setDescription(const QString &desc);

	protected:
		virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

		virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
		virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
		virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
		virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
		virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
		virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

	signals:
		void removeRequest();
		void editRequest();

	private:
		XmpRegion m_region;
};

#endif
