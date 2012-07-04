#include <QPainter>
#include <QPaintEvent>

#include "PTagWidget.h"

#include <QDebug>

PTagWidget::PTagWidget(QWidget *parent) : QWidget(parent)
{
	hide();
}

PTagWidget::PTagWidget(const PTag &tag, QSize imgSize, QWidget *parent)
	: QWidget(parent)
{
	hide();
	m_ptag = tag;
	m_imgSize = imgSize;
}

PTagWidget::~PTagWidget()
{
}

void PTagWidget::setFrameSize(int w, int h)
{
	m_imgSize.setWidth(w);
	m_imgSize.setHeight(h);
	updateRect();
}

void PTagWidget::setPTag(const PTag &tag)
{
	m_ptag = tag;
	updateRect();
}

QRect PTagWidget::tagRect() const
{
	return m_rect;
}

void PTagWidget::updateRect()
{
	QRectF rf(m_ptag.region().left() * m_imgSize.width(),
		  m_ptag.region().top()  * m_imgSize.height(),
		  m_ptag.region().width()  * m_imgSize.width(),
		  m_ptag.region().height() * m_imgSize.height());

	m_rect = rf.toRect();

	QFontMetrics fm = this->fontMetrics();
	int fmW = fm.width(m_ptag.text());
	int fmH = fm.height();

	QRect wr(m_rect.x(), m_rect.y(), fmW, m_rect.height() + fmH + 1);

	if (fmW < m_rect.width()) {
		wr.setWidth(m_rect.width() + 1);
	}

	m_textRect = QRect(0, wr.height() - fmH, fmW, fmH);

	setGeometry(wr);
}

void PTagWidget::paintEvent(QPaintEvent *pe)
{
	QPainter painter;

	QRect trr(0, 0, m_rect.width(), m_rect.height());

	painter.begin(this);
	painter.setPen(QPen(Qt::red, 1, Qt::SolidLine));
	painter.drawRect(trr);

	painter.setPen(QPen(Qt::white, 1, Qt::SolidLine));
	painter.setBackground(QBrush(Qt::black, Qt::SolidPattern));
	painter.setBackgroundMode(Qt::OpaqueMode);

	painter.drawText(m_textRect, Qt::AlignCenter, m_ptag.text(), 0);
	painter.end();
}
