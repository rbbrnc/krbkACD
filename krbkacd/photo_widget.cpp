#include "photo_widget.h"
#include "ui_photo_widget.h"

//#include <math.h>
#include <QDebug>


PhotoWidget::PhotoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PhotoWidget)
{
	ui->setupUi(this);

	pixScene = new QGraphicsScene();
	ui->pixView->setScene(pixScene);

//	pixItem = new QGraphicsPixmapItem();
//	pixScene->addItem(pixItem);

	this->setMouseTracking(true);
	ui->pixView->setMouseTracking(true);



}

PhotoWidget::~PhotoWidget()
{
	delete pixScene;
	delete ui;
}

void PhotoWidget::setPixmap(QPixmap pix)
{
	pixScene->clear();

#if 10
	pixScene->addPixmap(pix);
#else
	pixItem->setPixmap(pix);
	pixScene->addItem(pixItem);
#endif

#if 0
	exiv2 = new QExiv2();
	if (exiv2->load(file)) {
		m_tagList = exiv2->xmpPTags();
		for (int i = 0; i < m_tagList.size(); i++) {
			m_tag = new PTagWidget(m_tagList.at(i), QSize(w, h), this);
			m_tagWidgetList.append(m_tag);
			m_tagList.at(i).debug();
		}
	}
#endif
}

void PhotoWidget::resizeEvent(QResizeEvent *event)
{
	int w = event->size().width();
	int h = event->size().height();

	qDebug() << "PhotoWidget:" << __func__ << "w:" << w << "h:" << h;
#if 0
	if (!m_pic.isNull()) {
		m_pic = m_pic.scaled(w, h, Qt::KeepAspectRatio);
		label->setPixmap(m_pic);
	}

	for (int i = 0; i < m_tagWidgetList.size(); i++) {
		m_tagWidgetList.at(i)->setFrameSize(w, h);
	 }
#endif
}

//
// Zoom the view in and out.
// cfr. http://www.qtcentre.org/wiki/index.php?title=QGraphicsView:_Smooth_Panning_and_Zooming
//
void PhotoWidget::wheelEvent(QWheelEvent *event)
{
	// Zoom only when CTRL key pressed
	if (event->modifiers().testFlag(Qt::ControlModifier)) {
		//Get the position of the mouse before scaling, in scene coords
		QPointF pointBeforeScale(ui->pixView->mapToScene(event->pos()));

		//Scale the view ie. do the zoom
		double scaleFactor = 1.15; //How fast we zoom
		if (event->delta() > 0) {
			//Zoom in
			ui->pixView->scale(scaleFactor, scaleFactor);
		} else {
			//Zooming out
			ui->pixView->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
		}
		event->accept();
	}
}

void PhotoWidget::mouseMoveEvent(QMouseEvent *event)
{
	int x = event->x();
	int y = event->y();

	qDebug() << "PhotoWidget:" << __func__ << "x:" << x << "y:" << y;
#if 0
	for (int i = 0; i < m_tagWidgetList.size(); i++) {
		PTagWidget *t = m_tagWidgetList.at(i);
		QRect tagRect = t->tagRect();

		if (tagRect.contains(x, y) && t->isHidden()) {
//			pixScene->addRect(x,y,h,w);
			t->show();
		} else {
			if (t->isVisible()) {
				t->hide();
			}
		}
	 }
#endif
}
