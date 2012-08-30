#include <QBrush>
#include <QLinearGradient>
#include <QApplication>

#include <QDebug>

#include "PTagGraphicsItem.h"
#include "math.h"

PTagGraphicsItem::PTagGraphicsItem(const PTag &tag, QSize imgSize) :
	m_ptag(tag),
	m_imgSize(imgSize),
	m_visible(false)
{
	updateRect();
	setAcceptHoverEvents(true);
}

void PTagGraphicsItem::updateRect()
{
	QRectF rf(m_ptag.region().left() * m_imgSize.width(),
		  m_ptag.region().top()  * m_imgSize.height(),
		  m_ptag.region().width()  * m_imgSize.width(),
		  m_ptag.region().height() * m_imgSize.height());

	m_rect = rf.toRect();

#if 0
	qDebug() << __PRETTY_FUNCTION__
		 << "m_ptag:" << m_ptag.region()
		 << "rf:" << rf
		 << "m_rect:" << m_rect;
#endif

	QFontMetrics fm(QApplication::font());
	int fmW = fm.width(m_ptag.name());
	int fmH = fm.height();

	QRect wr(m_rect.x(), m_rect.y(), fmW, m_rect.height() + fmH + 1);

	if (fmW < m_rect.width()) {
		wr.setWidth(m_rect.width() + 1);
	}

	m_textRect = QRect(0, wr.height() - fmH, fmW, fmH);

#if 10
	m_width  = wr.width();
	m_height = wr.height();
#else
	m_width  = rf.width();
	m_height = rf.height();
#endif
	// Set the position of the Tag Box on the scene.
	setPos(wr.x(), wr.y());
}

void PTagGraphicsItem::setVisibility(enum PTagGraphicsItemVisibility visibility)
{
	switch (visibility) {
	case visibilityNormal:
		m_visible = true;
		setAcceptHoverEvents(false);
		break;
	case visibilityHover:
		m_visible = false;
		setAcceptHoverEvents(true);
		break;
	case visibilityNone:
		m_visible = false;
		setAcceptHoverEvents(false);
		break;
	}
	update(0, 0, m_width, m_height);
}

void PTagGraphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
	m_visible = false;
	update(0, 0, m_width, m_height);
}

void PTagGraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *)
{
	m_visible = true;
	update(0, 0, m_width, m_height);
}

// boundingRect must be re-implemented from the base class to provide the scene with
// size info about this custom GraphicsItem
// Provide the diminsions of the box to the QGraphicsView
QRectF PTagGraphicsItem::boundingRect() const
{
	return QRectF(0, 0, m_width, m_height);
}

void PTagGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	QRect trr(0, 0, m_rect.width(), m_rect.height());

	QColor borderColor;
	QColor textColor;
	QColor textBackgroundColor;

	if (m_visible) {
		borderColor = Qt::red;
		textColor   = Qt::white;
		textBackgroundColor = Qt::black;
	} else {
		borderColor = Qt::transparent;
		textColor   = Qt::transparent;
		textBackgroundColor = Qt::transparent;
	}

	m_pen.setColor(borderColor);
	m_pen.setStyle(Qt::SolidLine);

	painter->setPen(m_pen);
	painter->drawRect(trr);

	painter->setPen(QPen(textColor, 1, Qt::SolidLine));
	painter->setBackground(QBrush(textBackgroundColor, Qt::SolidPattern));
	painter->setBackgroundMode(Qt::OpaqueMode);
	painter->drawText(m_textRect, Qt::AlignCenter, m_ptag.name(), 0);
}

void PTagGraphicsItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	QMenu *menu = new QMenu;
	menu->addAction("Action 1");
	menu->addAction("Action 2");
	menu->popup(event->screenPos());
//	QObject::connect(menu, SIGNAL(triggered(QAction *)),
//			 this, SLOT(triggered(QAction *)));
}
