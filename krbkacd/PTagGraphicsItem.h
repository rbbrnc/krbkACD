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
		QGraphicsTextItem _text;    ///< sample text to go in the title area.

		void setFrameSize(int w, int h);

	protected:
		virtual QRectF boundingRect() const;
		virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
		virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
		virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

		QColor  m_borderColor;
		QPen    m_pen;

		qreal   _width;
		qreal   _height;

	private:
		void updateRect();

	private:
		QSize  m_imgSize;	// Whole frame
		QRect  m_rect;		// Actual region rect (screen coordinates)
		QRect  m_textRect;	// Rect for description Tag

		PTag	m_ptag;
};

#endif // PTAG_GRAPHICS_ITEM_H
