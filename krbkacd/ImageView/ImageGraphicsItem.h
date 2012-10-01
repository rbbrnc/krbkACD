#ifndef IMAGE_VIEW_H
#define IMAGE_VIEW_H

#include <QGraphicsPixmapItem>

class ImageGraphicsItem : public QGraphicsPixmapItem
{
	public:
		explicit ImageGraphicsItem(QGraphicsItem *parent = 0);
		~ImageGraphicsItem();

		void showImageRegions(bool show);

	private:
		//QImage m_image;
};

#endif
