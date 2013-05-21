#include <QtGui>

#include <QDebug>

#include "ImageViewManager.h"
#include "ImageGraphicsItem.h"
#include "ImageGraphicsView.h"
#include "RegionGraphicsItem.h"
#include "RegionEditDialog.h"

#ifdef USE_QEXIV2
#include "QExiv2.h"
#endif

ImageViewManager::ImageViewManager(QWidget *parent)
	: QWidget(parent),
	  m_image(0),
	  m_showRegions(false),
	  m_updateRegion(false)
{
	m_scene = new QGraphicsScene(this);
//	m_scene->setForegroundBrush(QBrush(Qt::yellow, Qt::NoBrush));

	m_view  = new ImageGraphicsView(this);
//	m_view->setForegroundBrush(QBrush(Qt::yellow, Qt::NoBrush));

	m_view->setScene(m_scene);
	m_view->setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));
//	m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
//	m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

	connect(m_view, SIGNAL(newRectRegion(const QRectF &)), this, SLOT(addRegion(const QRectF &)));
	connect(m_scene, SIGNAL(changed(const QList<QRectF> &)), this, SLOT(sceneChanged(const QList<QRectF> &)));
}

ImageViewManager::~ImageViewManager()
{
#ifdef USE_QEXIV2
	if (m_exiv2) {
		saveImageRegions();
		delete m_exiv2;
	}
#endif

	if (m_image) {
		delete m_image;
	}

	delete m_scene;
	delete m_view;
}

// [SLOT] public
void ImageViewManager::zoomIn()
{
	m_view->zoom(ImageGraphicsView::ZoomIn);
}
// [SLOT] public
void ImageViewManager::zoomOut()
{
	m_view->zoom(ImageGraphicsView::ZoomOut);
}

// [SLOT] public
// Zoom to original size [1:1]
void ImageViewManager::zoom11()
{
	m_view->zoom(ImageGraphicsView::ZoomOriginal);
}

// [SLOT] public
void ImageViewManager::zoomToFit()
{
	m_view->zoom(ImageGraphicsView::ZoomToFit);
}

// [SLOT] public
void ImageViewManager::rotateCCW()
{
	m_view->rotate(ImageGraphicsView::Rotation90CCW);
}

// [SLOT] public
void ImageViewManager::rotateCW()
{
	m_view->rotate(ImageGraphicsView::Rotation90CW);
}

// [SLOT] public
//  Reset zoom and rotations
void ImageViewManager::resetView()
{
	m_view->reset();
}

// [SLOT private]
void ImageViewManager::sceneChanged(const QList<QRectF> &region)
{
	if (region.count() <= 0) {
		return;
	}

	// Don't resize the image if is totally contained in the scene rect.
	//if (zoomToFitOption) {
		QSize viewSize = m_view->viewport()->size();
		if ((viewSize.width()  < region.at(0).toRect().width()) ||
		    (viewSize.height() < region.at(0).toRect().height())) {
			m_view->zoom(ImageGraphicsView::ZoomToFit);
		}
	//}
}

// [SLOT public]
void ImageViewManager::setFile(const QString &fileName)
{
#ifdef USE_QEXIV2
	saveImageRegions();
#endif
	QPixmap pixmap;
	if (!pixmap.load(fileName)) {
		setImage(QPixmap());
	} else {
		setImage(pixmap);
#ifdef USE_QEXIV2
		if (!m_exiv2) {
			m_exiv2 = new QExiv2();
		}
		setImageRegions(fileName);
		showRegions(m_showRegions);
#endif
	}
}

// [SLOT public]
void ImageViewManager::setImage(const QPixmap &pixmap)
{
	if (m_image) {
		delete m_image;
		m_scene->clear();
		m_view->reset();
#ifdef USE_QEXIV2
		m_regionHash.clear();
#endif
	}

	m_image = new ImageGraphicsItem(pixmap);
	m_scene->addItem(m_image);

	// QGraphicsScene's boundingRect will grow when items are added,
	// but doesn't shrink when items are removed.
	m_scene->setSceneRect(m_scene->itemsBoundingRect());

	if (pixmap.isNull()) {
		QGraphicsTextItem *ti = new QGraphicsTextItem("Not a Pixmap");
		ti->setDefaultTextColor(Qt::white);
		m_scene->addItem(ti);
	}
}

// [SLOT public]
// checked = true -> Set image panning mode
// checked = true -> Set region select mode.
void ImageViewManager::enableRegionSelection(bool enable)
{
	if (enable) {
		m_view->setCursor(Qt::ArrowCursor);
		m_view->setDragMode(QGraphicsView::RubberBandDrag);
		m_view->setInteractive(true);
#ifdef USE_QEXIV2
		showRegions(true);
#endif
	} else {
		m_view->setCursor(Qt::OpenHandCursor);
		m_view->setDragMode(QGraphicsView::ScrollHandDrag);
		m_view->setInteractive(false);
#ifdef USE_QEXIV2
		showRegions(false);
#endif
	}
}

// [SLOT public]
// called for new created regions
// rect = rubberband selection
void ImageViewManager::addRegion(const QRectF &rect)
{
	if (!rect.isValid()) {
		return;
	}

	RegionGraphicsItem *ir = new RegionGraphicsItem(rect);
	connect(ir, SIGNAL(removeRequest()), this, SLOT(removeRegion()));
	connect(ir, SIGNAL(editRequest()),   this, SLOT(editRegion()));

	m_scene->addItem(ir);
//	m_regionHash.insert(ir, region);

#if 0
	QSize imageSize;
	imageSize.setWidth(m_image->boundingRect().width());
	imageSize.setHeight(m_image->boundingRect().height());
#endif
	m_updateRegion = true;
}

// [SLOT private]
void ImageViewManager::removeRegion()
{
	RegionGraphicsItem *ri = dynamic_cast<RegionGraphicsItem *>(sender());

	QString msg = "Do you want to remove ";
	if (ri->name().isNull()) {
		msg += "this";
	} else {
		msg += ri->name();
	}
	msg += " region?";

	int rc = QMessageBox::question(this, "Remove Region", msg,
			 QMessageBox::Ok | QMessageBox::Cancel,
			 QMessageBox::Cancel);

	if (QMessageBox::Cancel == rc) {
		return;
	}

//	m_regionHash.remove(ri);
	m_scene->removeItem(dynamic_cast<QGraphicsItem *>(ri));
	m_updateRegion = true;
}

// [SLOT private]
void ImageViewManager::editRegion()
{
	RegionGraphicsItem *ri = dynamic_cast<RegionGraphicsItem *>(sender());
	RegionEditDialog dlg(ri, this);
	if (QDialog::Accepted == dlg.exec()) {
		qDebug() << "Name:" << ri->name() << "Desc:" << ri->description();
		m_updateRegion = true;
	}
}

// [SLOT public]
void ImageViewManager::showRegions(bool show)
{
	if (m_showRegions == show) {
		return;
	}
#if 0
	if (!m_regionHash.isEmpty()) {
		QHashIterator<RegionGraphicsItem *, XmpRegion> i(m_regionHash);
		while (i.hasNext()) {
			i.next();
			i.key()->setZValue((show) ? 1 : -1);
		}
	}
#endif
	m_showRegions = show;
}

#ifdef USE_QEXIV2
bool ImageViewManager::saveImageRegions()
{
	qDebug() << __PRETTY_FUNCTION__ << "Update:" << m_updateRegion;
	if ((!m_updateRegion) || (!m_exiv2)) {
		// No update needed
		return true;
	}

	QList<XmpRegion> rl;
	if (!m_regionHash.isEmpty()) {
		QHashIterator<RegionGraphicsItem *, XmpRegion> i(m_regionHash);
		while (i.hasNext()) {
			i.next();
			rl.append(i.value());
		}
	}

	if (m_exiv2->setXmpRegionList(rl)) {
		qDebug() << __PRETTY_FUNCTION__ << "Call save";
		m_updateRegion = false;
		return m_exiv2->save();
	}

	return false;
}

#endif
