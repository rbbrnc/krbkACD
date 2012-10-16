#include <QtGui>
//#include <QDebug>

#include "ImageGraphicsItem.h"

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

/*
void ImageGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	QPointF mousePosition = event->pos();
	QRgb rgbValue = pixmap().toImage().pixel(mousePosition.x(), mousePostion.y());

	emit currentPositionRgbChanged(mousePosition, rgbValue);
}
*/
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
