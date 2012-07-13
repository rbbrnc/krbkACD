#ifndef PHOTO_WIDGET_H
#define PHOTO_WIDGET_H

//#include <QWidget>
#include <QGraphicsScene>
//#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QMouseEvent>

#include "file_data.h"
#include "QExiv2.h"
#include "PTagWidget.h"
#include "PTagGraphicsItem.h"

namespace Ui {
	class PhotoWidget;
}

class PhotoWidget : public QWidget
{
	Q_OBJECT

	public:
		explicit PhotoWidget(QWidget *parent = 0);
		~PhotoWidget();

		void setFileData(FileData fdata);

        protected:
                void resizeEvent(QResizeEvent *event);
		void wheelEvent(QWheelEvent *event);

	private:
		Ui::PhotoWidget *ui;

		QGraphicsScene      *pixScene;
		QGraphicsPixmapItem *pixItem;

                QPixmap m_pic;

                PTagWidget *m_tag;
                QList<PTagWidget *> m_tagWidgetList;

                PTagGraphicsItem *m_tagItem;
                QList<PTagGraphicsItem *> m_tagItemList;

                QExiv2      m_metadata;
                QList<PTag> m_tagList;

};

#endif // PHOTO_WIDGET_H
