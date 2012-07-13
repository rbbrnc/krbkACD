#include "photo_widget.h"
#include "ui_photo_widget.h"

#include <QDebug>


PhotoWidget::PhotoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PhotoWidget)
{
	ui->setupUi(this);

	pixScene = new QGraphicsScene(this);
	ui->pixView->setScene(pixScene);
}

PhotoWidget::~PhotoWidget()
{
	delete pixScene;
	delete ui;
}

void PhotoWidget::setFileData(FileData fdata)
{
	pixScene->clear();

	m_pic = fdata.fullPixmap();
	pixScene->addPixmap(m_pic);

	int w = m_pic.width();
	int h = m_pic.height();

	m_metadata = fdata.metadata();
	m_tagList = m_metadata.xmpPTags();

	for (int i = 0; i < m_tagList.size(); i++) {
		m_tagItem = new PTagGraphicsItem(m_tagList.at(i), QSize(w, h));
		m_tagItemList.append(m_tagItem);
		pixScene->addItem(m_tagItem);
		//m_tagList.at(i).debug();
	}
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
#endif
	for (int i = 0; i < m_tagItemList.size(); i++) {
		m_tagItemList.at(i)->setFrameSize(w, h);
	 }
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
