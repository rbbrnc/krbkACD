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
	m_text = "<noname>";
}

void ImageRegionItem::setText(const QString &text)
{
	m_text = text;
}

void ImageRegionItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	qDebug() << __func__ << boundingRect();
	QToolTip::showText(event->screenPos(), m_text);
//	QGraphicsRectItem::hoverEnterEvent(event);
}

void ImageRegionItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
//	qDebug() << __func__ << event->pos();
//	QToolTip::showText(event->pos(), "text");
//	QGraphicsRectItem::hoverMoveEvent(event);
}

void ImageRegionItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
//	qDebug() << __func__;
	QToolTip::hideText();
//	QGraphicsRectItem::hoverLeaveEvent(event);
}


void ImageRegionItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	qDebug() << __func__;
	QToolTip::showText(event->screenPos(), m_text);
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

void ImageRegionItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	bool ok;
	QString s = QInputDialog::getText(0, "Rename", "New Name: ", QLineEdit::Normal,	m_text, &ok);

	if (ok) {
		m_text = s;
	}
	qDebug() << m_text;
}
