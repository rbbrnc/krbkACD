#ifndef IMAGE_GRAPHICS_ITEM_H
#define IMAGE_GRAPHICS_ITEM_H

#include <QGraphicsPixmapItem>

class ImageGraphicsItem : public QGraphicsPixmapItem
{
	public:
		ImageGraphicsItem(QGraphicsItem *parent = 0);
		ImageGraphicsItem(const QPixmap &pixmap, QGraphicsItem *parent = 0);
		~ImageGraphicsItem();

	protected:
		virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
		virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
		virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif
