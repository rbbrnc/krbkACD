#include <QtGui>
#include <QDebug>

#include "RegionGraphicsItem.h"

RegionGraphicsItem::RegionGraphicsItem(const QRectF &rect, QGraphicsItem *parent)
	: QGraphicsRectItem(rect, parent)
{
	m_type = 0;
	m_removeAction = m_menu.addAction("Remove");
	m_editAction   = m_menu.addAction("Edit");

	setPen(QColor(Qt::yellow));

	setAcceptHoverEvents(true);
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
	QGraphicsRectItem::mouseMoveEvent(event);
}

// [EVENT protected]
void RegionGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsRectItem::mousePressEvent(event);
}

// [EVENT protected]
void RegionGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
{
//	QGraphicsRectItem::mouseReleaseEvent(event);
}

// [EVENT protected]
void RegionGraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *)
{
	QString tip = "<b>" + m_name + "</b>";
	if (!m_description.isEmpty()) {
		tip += "<p>" + m_description + "</p>";
	}

	QGraphicsView *v = scene()->views().first();
	QPoint viewPos = v->mapFromScene(mapToScene(boundingRect().bottomLeft()));
	QToolTip::showText(v->viewport()->mapToGlobal(viewPos), tip);
//	QGraphicsItem::hoverEnterEvent(event);
}

// [EVENT protected]
void RegionGraphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
	QToolTip::hideText();
//	QGraphicsItem::hoverLeaveEvent(event);
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

int RegionGraphicsItem::type() const
{
	return m_type;
}

void RegionGraphicsItem::setType(int type)
{
	m_type = type;
}

// [EVENT protected]
void RegionGraphicsItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	QAction *action = m_menu.exec(event->screenPos());
	if (action == m_removeAction) {
		emit removeRequest();
	} else if (action == m_editAction) {
		emit editRequest();
	} else {
		// Nothing to do
	}
}
