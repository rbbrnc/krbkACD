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

FileData PhotoWidget::fileData() const
{
	return m_fileData;
}

void PhotoWidget::setFileData(FileData fdata)
{
	pixScene->clear();
	resetTransformations();

	m_fileData = fdata;

	m_currentFile = m_fileData.filePath();
	m_pic     = fdata.fullPixmap();
	m_pixItem = pixScene->addPixmap(m_pic);

	ui->pixView->centerOn(m_pixItem);

	int w = m_pic.width();
	int h = m_pic.height();

	m_metadata = fdata.metadata();

	// Check Microsoft MP regions
	m_tagList  = m_metadata->xmpPTags();
	if (m_tagList.isEmpty()) {
		// Check MWG regions
		m_tagList  = m_metadata->xmpMWG_RegionsTags();
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

		if (event->delta() > 0) {
			zoomIn();
		} else {
			zoomOut();
		}
		event->accept();
	}
}

void PhotoWidget::zoomIn()
{
	// Scale the view ie. do the zoom, scale factor = 1.15 aka how fast we zoom
	ui->pixView->scale(1.15, 1.15);
}

void PhotoWidget::zoomOut()
{
	// Scale the view ie. do the zoom, scale factor = 1.15 aka how fast we zoom
	ui->pixView->scale(1.0/1.15, 1.0/1.15);
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
//	qDebug() << __PRETTY_FUNCTION__ << "sceneRect:" << ui->pixView->sceneRect();

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

void PhotoWidget::deleteFile()
{
	::deleteFile(m_currentFile, this);
}

void PhotoWidget::renameFile()
{
	::renameFile(m_currentFile, this);
}

void PhotoWidget::showRegions(bool show)
{
	PTagGraphicsItem::PTagGraphicsItemVisibility v;

	if (show) {
		v = PTagGraphicsItem::visibilityNormal;
	} else {
		v = PTagGraphicsItem::visibilityHover;
	}

	for (int i = 0; i < m_tagItemList.size(); i++) {
		m_tagItemList.at(i)->setVisibility(v);
	}
}

void PhotoWidget::debugAction()
{
	qDebug() << __PRETTY_FUNCTION__;
}
