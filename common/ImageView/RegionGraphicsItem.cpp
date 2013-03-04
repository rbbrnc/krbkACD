#include <QtGui>
#include <QDebug>
#include <QMenu>

#include "RegionGraphicsItem.h"

RegionGraphicsItem::RegionGraphicsItem(const XmpRegion &region, QGraphicsItem *parent)
	: QGraphicsRectItem(region.boundingRect(), parent),
	  m_region(region)
{
//	setFlag(QGraphicsItem::ItemIsSelectable, true);
//	setAcceptHoverEvents(true);
//	setZValue(-1);
}

RegionGraphicsItem::~RegionGraphicsItem()
{
}

void RegionGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
#if 0
	QColor borderColor;
	borderColor = Qt::red;
	QPen pen;
	pen.setColor(borderColor);
	painter->setPen(pen);
	painter->drawRect(m_region.boundingRect());
#else
	QGraphicsRectItem::paint(painter, option, widget);
#endif
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
	qDebug() << __PRETTY_FUNCTION__;
	QGraphicsRectItem::mousePressEvent(event);
}

// [EVENT protected]
void RegionGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	Q_UNUSED(event)
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
	return m_region.name();
}

void RegionGraphicsItem::setName(const QString &name)
{
	m_region.setName(name);
}

QString RegionGraphicsItem::description() const
{
	return m_region.description();
}

void RegionGraphicsItem::setDescription(const QString &desc)
{
	m_region.setDescription(desc);
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
			 << m_region.name()
			 << m_region.description();
	}
}
