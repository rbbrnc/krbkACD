#include <QtGui>
#include <QDebug>

#include "ImageGraphicsItem.h"

ImageGraphicsItem::ImageGraphicsItem(QGraphicsItem *parent)
{
	rubberBand = 0;
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
//	qDebug() << __PRETTY_FUNCTION__;
#if 10
	QRectF inside = QGraphicsPixmapItem::boundingRect();
	QPointF mapPoint = mapFromScene(event->pos());
//	if (inside.contains(mapPoint))
//		rubberBand->setGeometry(QRect(origin, event->screenPos()).normalized());
#else
//	QGraphicsPixmapItem::mouseMoveEvent(event);
#endif
}

void ImageGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
//	qDebug() << __PRETTY_FUNCTION__ << "POS:" << event->pos();
#if 0
        origin = event->screenPos();
        if (!rubberBand)
            rubberBand = new QRubberBand(QRubberBand::Rectangle,0);

        rubberBand->setGeometry(QRect(origin, QSize()));
        rubberBand->show();
	qDebug() << __PRETTY_FUNCTION__ << "POS:" << event->pos();
#else

	m_selection = QRectF(event->pos(), QSizeF());

	QGraphicsPixmapItem::mousePressEvent(event);
#endif
}

void ImageGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
//	qDebug() << __PRETTY_FUNCTION__ << "POS:" << event->pos();
#if 0
	qDebug() << __PRETTY_FUNCTION__ << "POS:" << event->pos() << "Geometry:" << rubberBand->geometry();
	rubberBand->hide();
#else
	if (event->pos().x() > m_selection.x())
		m_selection.setWidth(event->pos().x() - m_selection.x());

	qDebug() << __PRETTY_FUNCTION__ << "SEL:" << m_selection;

//	QGraphicsPixmapItem::mouseReleaseEvent(event);
#endif
}

void ImageGraphicsItem::showImageRegions(bool show)
{

}


