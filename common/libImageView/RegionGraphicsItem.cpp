#include <QtGui>
#include <QDebug>
#include <QMenu>

#include "RegionGraphicsItem.h"

RegionGraphicsItem::RegionGraphicsItem(const QRectF &rect, QGraphicsItem *parent)
	: QGraphicsRectItem(rect, parent)
{
	setPen(QColor(Qt::yellow));
//	setFlag(QGraphicsItem::ItemIsSelectable, true);
//	setAcceptHoverEvents(true);
//	setZValue(-1);
//	setZValue(1);
}

RegionGraphicsItem::~RegionGraphicsItem()
{
}

void RegionGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QGraphicsRectItem::paint(painter, option, widget);
}

// [EVENT protected]
void RegionGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	qDebug() << __PRETTY_FUNCTION__;
	QGraphicsRectItem::mouseMoveEvent(event);
}

// [EVENT protected]
void RegionGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsRectItem::mousePressEvent(event);
}

// [EVENT protected]
void RegionGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	Q_UNUSED(event)
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
	return m_description;
}

void RegionGraphicsItem::setDescription(const QString &desc)
{
	m_description = desc;
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
			 << m_description;
	}
}
