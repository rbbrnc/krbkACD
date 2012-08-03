#ifndef PHOTO_WIDGET_H
#define PHOTO_WIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QMouseEvent>

#include "file_data.h"
#include "QExiv2.h"
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

	signals:
		void changePage(int);

	public slots:
		void zoom11();
		void zoomToFit();
		void rotateCW();
		void rotateCCW();
		void resetTransformations();
		void renameFile();
		void deleteFile();

        protected:
		void wheelEvent(QWheelEvent *event);
		virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
	private:
		Ui::PhotoWidget *ui;

		QGraphicsScene      *pixScene;
		QGraphicsPixmapItem *m_pixItem;

                QPixmap m_pic;

                PTagGraphicsItem *m_tagItem;
                QList<PTagGraphicsItem *> m_tagItemList;

                QExiv2      m_metadata;
                QList<PTag> m_tagList;
};

#endif // PHOTO_WIDGET_H
