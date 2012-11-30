#include <QtGui>
#include <QDebug>

#include "RegionGraphicsItem.h"

RegionGraphicsItem::RegionGraphicsItem(QGraphicsItem *parent)
	: QGraphicsRectItem(parent)
{
	init();
}

RegionGraphicsItem::RegionGraphicsItem(const QRectF &rect, QGraphicsItem *parent)
	: QGraphicsRectItem(rect, parent)
{
	init();
}

RegionGraphicsItem::RegionGraphicsItem(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent)
	: QGraphicsRectItem(x, y, width, height, parent )
{
	init();
}

void RegionGraphicsItem::init()
{
//	setFlag(QGraphicsItem::ItemIsSelectable, true);
//	setAcceptHoverEvents(true);
//	setZValue(-1);
}

RegionGraphicsItem::~RegionGraphicsItem()
{
}

// [EVENT protected]
void RegionGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	qDebug() << __PRETTY_FUNCTION__;
	QGraphicsRectItem::mouseMoveEvent(event);
}

// [EVENT protected]
void RegionGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	qDebug() << __PRETTY_FUNCTION__;
	QGraphicsRectItem::mousePressEvent(event);
}

// [EVENT protected]
void RegionGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	qDebug() << __PRETTY_FUNCTION__;
//	QGraphicsRectItem::mouseReleaseEvent(event);
}

// [EVENT protected]
void RegionGraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	qDebug() << __PRETTY_FUNCTION__ << zValue();
	//setZValue(1);
	QGraphicsItem::hoverEnterEvent(event);
}

// [EVENT protected]
void RegionGraphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	qDebug() << __PRETTY_FUNCTION__ << zValue();
	//setZValue(-1);
	QGraphicsItem::hoverLeaveEvent(event);
}

QString RegionGraphicsItem::name() const
{
	return m_name;
}

void RegionGraphicsItem::setName(const QString &name)
{
	m_name = name;
}

QString RegionGraphicsItem::description() const
{
	return m_text;
}

void RegionGraphicsItem::setDescription(const QString &desc)
{
	m_text = desc;
}

bool RegionGraphicsItem::isNormalized() const
{
	return m_normalized;
}

void RegionGraphicsItem::setNormalized(bool normalized)
{
	m_normalized = normalized;
}

// [EVENT protected]
void RegionGraphicsItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	QMenu menu;
	QAction *removeAction   = menu.addAction("Remove");
	QAction *editAction     = menu.addAction("Edit");
	QAction *selectedAction = menu.exec(event->screenPos());

	if (selectedAction == removeAction) {
		emit removeRequest();
	} else if (selectedAction == editAction) {
		emit editRequest();
	} else {
		// Nothing to do
		qDebug() << __PRETTY_FUNCTION__
			 << "NOP"
			 << m_name
			 << m_text;
	}
}
