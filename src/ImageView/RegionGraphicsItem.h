#ifndef REGION_GRAPHICS_ITEM_H
#define REGION_GRAPHICS_ITEM_H

#include <QGraphicsRectItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>

class RegionGraphicsItem : public QObject, public QGraphicsRectItem
{
	Q_OBJECT

	public:
        RegionGraphicsItem(const QRectF &rect = QRectF(), QGraphicsItem *parent = 0);

		~RegionGraphicsItem();

		QString name() const;
		void setName(const QString &name);

		QString description() const;
		void setDescription(const QString &desc);

		int type() const;
		void setType(int type);

	protected:
		virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

		virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
		virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
		virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
		virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
		virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
		virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

	signals:
		void removeRequest();
		void editRequest();

	private:
		QString m_name;
		QString m_description;
		int m_type;

		// Context Menu
		QMenu    m_menu;
		QAction *m_removeAction;
		QAction *m_editAction;
};

#endif
