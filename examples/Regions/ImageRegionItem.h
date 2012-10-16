#ifndef IMAGE_REGION_RECT_ITEM_H
#define IMAGE_REGION_RECT_VIEW_H

#include <QGraphicsRectItem>

class ImageRegionItem : public QGraphicsRectItem
{
	public:
		ImageRegionItem(QGraphicsItem *parent = 0);
		ImageRegionItem(const QRectF &rect, QGraphicsItem *parent = 0);
		ImageRegionItem(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent = 0);
		~ImageRegionItem();

		void setText(const QString &text);

	protected:

		virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
		virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
		virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);

		virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
		virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
		virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
		virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
	private:
		void init();

		QString m_text;
};

#endif
