#include <QtGui>
//#include <QDebug>

#include "ImageRegionItem.h"

ImageRegionItem::ImageRegionItem(QGraphicsItem *parent)
	: QGraphicsRectItem(parent)
{
	init();
}

ImageRegionItem::ImageRegionItem(const QRectF &rect, QGraphicsItem *parent)
	: QGraphicsRectItem(rect, parent)
{
	init();
}

ImageRegionItem::ImageRegionItem(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent)
	: QGraphicsRectItem(x, y, width, height, parent)
{
	init();
}

ImageRegionItem::~ImageRegionItem()
{
}

void ImageRegionItem::init()
{
	setAcceptHoverEvents(true);
}

void ImageRegionItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	QToolTip::showText(event->screenPos(), "text");
	QGraphicsRectItem::hoverEnterEvent(event);
}

void ImageRegionItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
//	qDebug() << __func__;
	QToolTip::hideText();
	QGraphicsRectItem::hoverLeaveEvent(event);
}

/*
void ImageRegionItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsRectItem::mouseMoveEvent(event);
}

void ImageRegionItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsRectItem::mousePressEvent(event);
}

void ImageRegionItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsRectItem::mouseReleaseEvent(event);
}
*/
