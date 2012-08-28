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

#include <QMenu>
#include "PTag.h"

class PTagGraphicsItem : public QGraphicsItem
{
	public:
		PTagGraphicsItem(const PTag &tag, QSize imgSize);

		enum PTagGraphicsItemVisibility {
			visibilityNormal = 0,	// Tag is always visible
			visibilityHover,	// Tag is visible on hoower
			visibilityNone,		// Tag is always hide
		};

		void setVisibility(enum PTagGraphicsItemVisibility visibility);

	protected:
		virtual QRectF boundingRect() const;
		virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
		virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
		virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

		virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

	private:
		void updateRect();

	private:
		PTag	m_ptag;

		// Image size
		QSize  m_imgSize;

		// Actual region rect (in screen coordinates)
		// tag rectangle + tag text
		QRect  m_rect;

		// Rect for description Tag
		QRect  m_textRect;
		QPen   m_pen;

		qreal   m_width;
		qreal   m_height;

		bool m_visible;
};

#endif // PTAG_GRAPHICS_ITEM_H
