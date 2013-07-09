#include <QtGui>

#include "ImageGraphicsItem.h"

#include <QDebug>

ImageGraphicsItem::ImageGraphicsItem(QGraphicsItem *parent)
	: QGraphicsPixmapItem(parent)
{
}

ImageGraphicsItem::ImageGraphicsItem(const QPixmap &pixmap, QGraphicsItem *parent)
	: QGraphicsPixmapItem(pixmap, parent)
{
}

ImageGraphicsItem::~ImageGraphicsItem()
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
