#include <QtGui>

#include "ImageGraphicsItem.h"

ImageGraphicsItem::ImageGraphicsItem(QGraphicsItem *parent)
	: QGraphicsPixmapItem(parent)
{
}

ImageGraphicsItem::ImageGraphicsItem(const QPixmap &pixmap, QGraphicsItem *parent)
	: QGraphicsPixmapItem(pixmap, parent)
{
}

void ImageGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsPixmapItem::mouseMoveEvent(event);
}

void ImageGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsPixmapItem::mousePressEvent(event);
}

void ImageGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsPixmapItem::mouseReleaseEvent(event);
}
