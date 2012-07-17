#ifndef PTAG_GRAPHICS_ITEM_H
#define PTAG_GRAPHICS_ITEM_H

#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QColor>
#include <QPainter>
#include <QPen>
#include <QPointF>

#include "PTag.h"

class PTagGraphicsItem : public QGraphicsItem
{
	public:
		PTagGraphicsItem(const PTag &tag, QSize imgSize);

	protected:
		virtual QRectF boundingRect() const;
		virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
		virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
		virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

	private:
		void updateRect();

	private:
		// Image size
		QSize  m_imgSize;

		// Actual region rect (in screen coordinates)
		// tag rectangle + tag text
		QRect  m_rect;

		// Rect for description Tag
		QRect  m_textRect;

		PTag	m_ptag;
///
		QColor  m_borderColor;
		QPen    m_pen;

		qreal   _width;
		qreal   _height;
};

#endif // PTAG_GRAPHICS_ITEM_H
