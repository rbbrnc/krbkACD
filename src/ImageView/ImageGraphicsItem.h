#ifndef IMAGE_GRAPHICS_ITEM_H
#define IMAGE_GRAPHICS_ITEM_H

#include <QGraphicsPixmapItem>

class QMovie;

class ImageGraphicsItem : public QGraphicsPixmapItem
{
	public:
		ImageGraphicsItem(QGraphicsItem *parent = 0);
		ImageGraphicsItem(const QPixmap &pixmap, QGraphicsItem *parent = 0);
		~ImageGraphicsItem();

        void setMovie(const QString &fileName);

	protected:
        virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
		virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
		virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    private:
        QMovie *m_movie;

};

#endif




