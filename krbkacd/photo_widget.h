#ifndef PHOTO_WIDGET_H
#define PHOTO_WIDGET_H

#include <QWidget>

#include <QGraphicsScene>
//#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QMouseEvent>

#if 10
#include "PTagWidget.h"
#include "QExiv2.h"
#endif

namespace Ui {
	class PhotoWidget;
}

class PhotoWidget : public QWidget
{
	Q_OBJECT

	public:
		explicit PhotoWidget(QWidget *parent = 0);
		~PhotoWidget();

		void setPixmap(QPixmap pix);

        protected:
                void resizeEvent(QResizeEvent *event);
		void wheelEvent(QWheelEvent *event);

        private:
                void mouseMoveEvent(QMouseEvent *);

	private:
		Ui::PhotoWidget *ui;

		QGraphicsScene      *pixScene;
		QGraphicsPixmapItem *pixItem;

#if 10
                QPixmap m_pic;

                PTagWidget *m_tag;
                QList<PTagWidget *> m_tagWidgetList;

                QExiv2 *exiv2;
                QList<PTag> m_tagList;
#endif
};

#endif // PHOTO_WIDGET_H
