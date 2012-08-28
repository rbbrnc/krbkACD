#include "photo_widget.h"
#include "ui_photo_widget.h"

#include <QDebug>

#include "file_utils.h"

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
	resetTransformations();

	m_currentFile = fdata.filePath();
	m_pic     = fdata.fullPixmap();
	m_pixItem = pixScene->addPixmap(m_pic);

	ui->pixView->centerOn(m_pixItem);

	int w = m_pic.width();
	int h = m_pic.height();

	m_metadata = fdata.metadata();

	// Check Microsoft MP regions
	m_tagList  = m_metadata.xmpPTags();
	if (m_tagList.isEmpty()) {
		// Check MWG regions
		m_tagList  = m_metadata.xmpMWG_RegionsTags();
	}

	for (int i = 0; i < m_tagList.size(); i++) {
		m_tagItem = new PTagGraphicsItem(m_tagList.at(i), QSize(w, h));
		m_tagItemList.append(m_tagItem);
		pixScene->addItem(m_tagItem);
		//m_tagList.at(i).debug();
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

// Zoom 1:1
void PhotoWidget::zoom11()
{
	QTransform t = ui->pixView->transform();

	if (t.isScaling()) {
		// Reset the scale transformation
		// m11 and m22 specifying the scale.
		t.setMatrix(1.0    , t.m12(), t.m13(),
		    t.m21(), 1.0    , t.m23(),
		    t.m31(), t.m32(), t.m33());

		ui->pixView->setTransform(t, false);
	}
}

// Zoom to fit
void PhotoWidget::zoomToFit()
{
//	ui->pixView->fitInView(m_pixItem, Qt::IgnoreAspectRatio);
//	ui->pixView->fitInView(m_pixItem, Qt::KeepAspectRatio);
//	ui->pixView->fitInView(m_pixItem, Qt::KeepAspectRatioByExpanding);
	qDebug() << __PRETTY_FUNCTION__ << "sceneRect:" << ui->pixView->sceneRect();

	ui->pixView->fitInView(ui->pixView->sceneRect(), Qt::KeepAspectRatioByExpanding);
}

void PhotoWidget::rotateCW()
{
	QTransform t = ui->pixView->transform();
	t.rotate(90.0, Qt::ZAxis);
	ui->pixView->setTransform(t, false);
}

void PhotoWidget::rotateCCW()
{
	QTransform t = ui->pixView->transform();
	t.rotate(-90.0, Qt::ZAxis);
	ui->pixView->setTransform(t, false);
}

void PhotoWidget::resetTransformations()
{
	ui->pixView->setTransform(QTransform(), false);
}

#if 10
void PhotoWidget::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	qDebug() << __PRETTY_FUNCTION__;
        QMenu *menu = new QMenu;
        QAction *action;
	action = menu->addAction("Rotate CW");
	connect(menu, SIGNAL(triggered()), this, SLOT(rotateCW()));

        action = menu->addAction("Rotate CCW");
	connect(menu, SIGNAL(triggered()), this, SLOT(rotateCCW()));

        menu->popup(event->screenPos());

	ui->pixView->setTransform(QTransform(), false);
//      QObject::connect(menu, SIGNAL(triggered(QAction *)),
//       this, SLOT(triggered(QAction *)));
}
#endif

void PhotoWidget::deleteFile()
{
	::deleteFile(m_currentFile, this);
}

void PhotoWidget::renameFile()
{
	::renameFile(m_currentFile, this);
}
