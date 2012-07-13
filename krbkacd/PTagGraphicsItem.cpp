#include <QDebug>
#include <QBrush>
#include <QLinearGradient>
#include <QApplication>

#include "PTagGraphicsItem.h"
#include "math.h"

PTagGraphicsItem::PTagGraphicsItem(const PTag &tag, QSize imgSize)
{
//	this->hide();

	m_ptag = tag;
	m_imgSize = imgSize;

#if 0
        m_borderColor = QColor(Qt::black);
	m_pen.setWidth(1);
	m_pen.setColor(m_borderColor);
#endif
	_text.setPlainText(m_ptag.text());
	_text.setParentItem(this);

	updateRect();

	this->setAcceptHoverEvents(true);
}

void PTagGraphicsItem::updateRect()
{
	QRectF rf(m_ptag.region().left() * m_imgSize.width(),
		  m_ptag.region().top()  * m_imgSize.height(),
		  m_ptag.region().width()  * m_imgSize.width(),
		  m_ptag.region().height() * m_imgSize.height());

	m_rect = rf.toRect();

//	qDebug() << __func__ << rf;

	QFontMetrics fm(QApplication::font());
	int fmW = fm.width(m_ptag.text());
	int fmH = fm.height();

	QRect wr(m_rect.x(), m_rect.y(), fmW, m_rect.height() + fmH + 1);

	if (fmW < m_rect.width()) {
		wr.setWidth(m_rect.width() + 1);
	}

	m_textRect = QRect(0, wr.height() - fmH, fmW, fmH);

#if 10
	_width  = wr.width();
	_height = wr.height();
#else
	_width  = rf.width();
	_height = rf.height();
#endif
	// Set the position of the Box
	this->setPos(wr.x(), wr.y());
}

void PTagGraphicsItem::setFrameSize(int w, int h)
{
	m_imgSize.setWidth(w);
	m_imgSize.setHeight(h);
	updateRect();
}

void PTagGraphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
	this->hide();
//	m_borderColor = Qt::black;
//	this->update(0, 0, _width, _height);
}

void PTagGraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *)
{
	m_borderColor = Qt::red;
//	this->update(0, 0, _width, _height);
	this->show();
}

// boundingRect must be re-implemented from the base class to provide the scene with
// size info about this custom GraphicsItem
// Provide the diminsions of the box to the QGraphicsView
QRectF PTagGraphicsItem::boundingRect() const
{
	return QRectF(0, 0, _width, _height);
}

void PTagGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	QRect trr(0, 0, m_rect.width(), m_rect.height());

//	qDebug() << __func__ << trr;

	m_pen.setColor(m_borderColor);
	m_pen.setStyle(Qt::SolidLine);

	painter->setPen(m_pen);

//	painter->setPen(QPen(Qt::red, 1, Qt::SolidLine));
	painter->drawRect(trr);

	painter->setPen(QPen(Qt::white, 1, Qt::SolidLine));
	painter->setBackground(QBrush(Qt::black, Qt::SolidPattern));
	painter->setBackgroundMode(Qt::OpaqueMode);
	painter->drawText(m_textRect, Qt::AlignCenter, m_ptag.text(), 0);
}
