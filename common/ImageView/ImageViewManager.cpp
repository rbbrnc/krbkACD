#include <QtGui>

#include <QDebug>

#include "ImageViewManager.h"
#include "ImageGraphicsItem.h"
#include "RegionGraphicsItem.h"
#include "RegionEditDialog.h"
#include "ImageGraphicsView.h"
#include "QExiv2.h"

ImageViewManager::ImageViewManager(QWidget *parent)
	: QWidget(parent),
	  m_image(0),
	  m_exiv2(0),
	  m_showRegions(false),
	  m_updateRegion(false)
{
	m_scene = new QGraphicsScene(this);
	m_view  = new ImageGraphicsView(this);

	m_view->setScene(m_scene);
	m_view->setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));

	QSize iconSize(32, 32);

	// Zoom In
	QToolButton *zoomInButton  = new QToolButton();
	zoomInButton->setIcon(QIcon(":/images/zoom_in.png"));
	zoomInButton->setIconSize(iconSize);

	// Zoom Out
	QToolButton *zoomOutButton = new QToolButton();
	zoomOutButton->setIcon(QIcon(":/images/zoom_out.png"));
	zoomOutButton->setIconSize(iconSize);

	// Zoom to Fit
	QToolButton *zoomToFitButton = new QToolButton();
	zoomToFitButton->setIcon(QIcon(":/images/zoom_best_fit.png"));
	zoomToFitButton->setIconSize(iconSize);

	// Zoom 1:1
	QToolButton *zoom11Button = new QToolButton();
	zoom11Button->setIcon(QIcon(":/images/zoom_original.png"));
	zoom11Button->setIconSize(iconSize);

	// Rotate CCW
	QToolButton *rotateCCWButton = new QToolButton();
	rotateCCWButton->setIcon(QIcon(":/images/rotate_ccw.png"));
	rotateCCWButton->setIconSize(iconSize);

	// Rotate CW
	QToolButton *rotateCWButton = new QToolButton();
	rotateCWButton->setIcon(QIcon(":/images/rotate_cw.png"));
	rotateCWButton->setIconSize(iconSize);

	// Reset View to original
	QToolButton *resetViewButton = new QToolButton();
	resetViewButton->setIcon(QIcon(":/images/original.png"));
	resetViewButton->setIconSize(iconSize);

	// View Mode for Panning image or for region selection.
	QToolButton *modeButton = new QToolButton();
	modeButton->setCheckable(true);
	modeButton->setIcon(QIcon(":/images/select.png"));
	modeButton->setIconSize(iconSize);

	// Previous Image
	QToolButton *previousButton = new QToolButton();
	previousButton->setIcon(QIcon(":/images/previous.png"));
	previousButton->setIconSize(iconSize);

	// Next Image
	QToolButton *nextButton = new QToolButton();
	nextButton->setIcon(QIcon(":/images/next.png"));
	nextButton->setIconSize(iconSize);

	QHBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(zoomInButton);
	buttonLayout->addWidget(zoomOutButton);
	buttonLayout->addWidget(zoom11Button);
	buttonLayout->addWidget(zoomToFitButton);
	buttonLayout->addWidget(rotateCCWButton);
	buttonLayout->addWidget(rotateCWButton);
	buttonLayout->addWidget(resetViewButton);
	buttonLayout->addWidget(modeButton);
	buttonLayout->addWidget(previousButton);
	buttonLayout->addWidget(nextButton);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(m_view);
	layout->addLayout(buttonLayout);
	setLayout(layout);

	connect(zoomInButton,    SIGNAL(clicked()), m_view, SLOT(zoomIn()));
	connect(zoomOutButton,   SIGNAL(clicked()), m_view, SLOT(zoomOut()));
	connect(zoom11Button,    SIGNAL(clicked()), m_view, SLOT(zoom11()));
	connect(zoomToFitButton, SIGNAL(clicked()), m_view, SLOT(zoomToFit()));

	connect(rotateCCWButton, SIGNAL(clicked()), m_view, SLOT(rotateCCW()));
	connect(rotateCWButton,  SIGNAL(clicked()), m_view, SLOT(rotateCW()));

	connect(resetViewButton, SIGNAL(clicked()), m_view, SLOT(reset()));

	connect(modeButton, SIGNAL(toggled(bool)), this, SLOT(enableRegionSelection(bool)));

	connect(previousButton, SIGNAL(clicked()), this, SLOT(previous()));
	connect(nextButton,     SIGNAL(clicked()), this, SLOT(next()));

	connect(m_scene, SIGNAL(changed(const QList<QRectF> &)), this, SLOT(sceneChanged(const QList<QRectF> &)));
	connect(m_view,  SIGNAL(newRectRegion(const QRectF &)), this, SLOT(addRectRegion(const QRectF &)));
}

ImageViewManager::~ImageViewManager()
{
	if (m_exiv2) {
		saveImageRegions();
		delete m_exiv2;
	}

	if (m_image) {
		delete m_image;
	}

	delete m_scene;
	delete m_view;
}

// [SLOT private]
// checked = true -> Set image panning mode
// checked = true -> Set region select mode.
void ImageViewManager::enableRegionSelection(bool enable)
{
	if (enable) {
		m_view->setCursor(Qt::ArrowCursor);
		m_view->setDragMode(QGraphicsView::RubberBandDrag);
		m_view->setInteractive(true);
		showImageRegions(true);
	} else {
		m_view->setCursor(Qt::OpenHandCursor);
		m_view->setDragMode(QGraphicsView::ScrollHandDrag);
		m_view->setInteractive(false);
		showImageRegions(false);
	}
}

// [SLOT private]
void ImageViewManager::sceneChanged(const QList<QRectF> &region)
{
	if (region.count() <= 0) {
		return;
	}

	//qDebug() << __PRETTY_FUNCTION__;
	// Don't resize the image if is totally contained in the scene rect.
//	if (zoomToFitOption) {
		QSize viewSize = m_view->viewport()->size();
		if ((viewSize.width()  < region.at(0).toRect().width()) ||
		    (viewSize.height() < region.at(0).toRect().height())) {
			m_view->zoomToFit();
		}
//	}
}

// [SLOT public]
void ImageViewManager::setFile(const QString &fileName)
{
	saveImageRegions();

	QPixmap pixmap;
	if (!pixmap.load(fileName)) {
		setImage(QPixmap());
	} else {
		setImage(pixmap);
		if (!m_exiv2) {
			m_exiv2 = new QExiv2();
		}
		setImageRegions(fileName);
		showImageRegions(m_showRegions);
	}
}

// [SLOT public]
void ImageViewManager::setImage(const QPixmap &pixmap)
{
	if (m_image) {
		delete m_image;
		m_scene->clear();
		m_view->reset();
		m_regionHash.clear();
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
void ImageViewManager::previous()
{
	emit requestPreviousFile();
}

// [SLOT public]
void ImageViewManager::next()
{
	emit requestNextFile();
}

// [SLOT public]
void ImageViewManager::showImageRegions(bool show)
{
	if (!m_regionHash.isEmpty()) {
		QHashIterator<RegionGraphicsItem *, XmpRegion> i(m_regionHash);
		while (i.hasNext()) {
			i.next();
			i.key()->setZValue((show) ? 1 : -1);
		}
	}

	m_showRegions = show;
}

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

// [private]
void ImageViewManager::addRectRegion(XmpRegion &region)
{
	RegionGraphicsItem *ir = new RegionGraphicsItem(region);

	connect(ir, SIGNAL(removeRequest()), this, SLOT(removeRectRegion()));
	connect(ir, SIGNAL(editRequest()), this, SLOT(editRectRegion()));

	m_scene->addItem(ir);
	m_regionHash.insert(ir, region);
}

// [SLOT private]
// called for new created regions
// rect = rubberband selection
void ImageViewManager::addRectRegion(const QRectF &rect)
{
	if (!rect.isValid()) {
		return;
	}

	XmpRegion r;
	r.setFormat(XmpRegion::MWG);

	QSize imageSize;
	imageSize.setWidth(m_image->boundingRect().width());
	imageSize.setHeight(m_image->boundingRect().height());

	r.setRegion(rect, imageSize);
	addRectRegion(r);

	qDebug() << __PRETTY_FUNCTION__;
	r.debug();

	m_updateRegion = true;
}

// [SLOT private]
void ImageViewManager::removeRectRegion()
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
			 QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel);

	if (QMessageBox::Cancel == rc) {
		return;
	}

	m_regionHash.remove(ri);
	m_scene->removeItem(dynamic_cast<QGraphicsItem *>(ri));
	m_updateRegion = true;
}

// [SLOT private]
void ImageViewManager::editRectRegion()
{
	RegionGraphicsItem *ri = dynamic_cast<RegionGraphicsItem *>(sender());

	if (m_regionHash.contains(ri)) {
		RegionEditDialog dlg(m_regionHash.value(ri), this);
		dlg.exec();
		// TODO:
		//m_updateRegion = true;
	}
}

void ImageViewManager::setImageRegions(const QString &fileName)
{
	m_updateRegion = false;
	if (!m_exiv2->load(fileName)) {
		return;
	}

	if (!m_exiv2->hasXmpRegionTag()) {
		return;
	}

	// Get XMP Image Regions
	QList<XmpRegion> rl = m_exiv2->xmpRegionList();
	for (int i = 0; i < rl.size(); i++) {
		//rl.at(i).debug();
		XmpRegion r = rl.at(i);
		addRectRegion(r);
	}
}
