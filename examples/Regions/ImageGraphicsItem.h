#ifndef IMAGE_VIEW_H
#define IMAGE_VIEW_H

#include <QGraphicsPixmapItem>

class ImageGraphicsItem : public QGraphicsPixmapItem
{
	public:
		explicit ImageGraphicsItem(QGraphicsItem *parent = 0);
		~ImageGraphicsItem();

		void showImageRegions(bool show);

	protected:
		virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
		virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
		virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

	private:
		QRubberBand * rubberBand;
		QPoint origin;

		QRectF m_selection;
};

#endif
