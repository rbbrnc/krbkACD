#include <QMovie>

#include "ImageGraphicsItem.h"

ImageGraphicsItem::ImageGraphicsItem(QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent),
      m_movie(Q_NULLPTR)
{
}

ImageGraphicsItem::ImageGraphicsItem(const QPixmap &pixmap, QGraphicsItem *parent)
    : QGraphicsPixmapItem(pixmap, parent),
      m_movie(Q_NULLPTR)
{
}

void ImageGraphicsItem::setMovie(const QString &fileName)
{
    if (m_movie) {
        delete m_movie;
    }
    m_movie = new QMovie(fileName);

    QObject::connect(m_movie, &QMovie::frameChanged, [=] { this->setPixmap(m_movie->currentPixmap());});
    m_movie->start();
}

ImageGraphicsItem::~ImageGraphicsItem()
{   
    if (m_movie) {
        delete m_movie;
    }
}

void ImageGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsPixmapItem::mouseMoveEvent(event);
}

void ImageGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsPixmapItem::mousePressEvent(event);
}

void ImageGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsPixmapItem::mouseReleaseEvent(event);
}
