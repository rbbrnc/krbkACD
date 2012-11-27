#include <QtGui>

#include <QDebug>

#include "ImageViewManager.h"
#include "ImageGraphicsItem.h"
#include "RegionGraphicsItem.h"
#include "ImageGraphicsView.h"

ImageViewManager::ImageViewManager(QWidget *parent)
	: QWidget(parent),
	  m_image(0)
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
	previousButton = new QToolButton();
	previousButton->setIcon(QIcon(":/images/previous.png"));
	previousButton->setIconSize(iconSize);

	// Next Image
	nextButton = new QToolButton();
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
	if (m_image) {
		delete m_image;
	}

	delete m_scene;
	delete m_view;
}

void ImageViewManager::updateButtons()
{
	int fcount = m_fileList.count();
	if (fcount > 0) {
		nextButton->setEnabled((m_currentFile < fcount -1) ? true : false);
		previousButton->setEnabled((m_currentFile > 0) ? true : false);
	} else {
		nextButton->setEnabled(false);
		previousButton->setEnabled(false);
	}
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

		qDebug() << __PRETTY_FUNCTION__ << "RegionListSize:" << m_regionList.size();
		for (int i = 0; i < m_regionList.size(); ++i) {
			//m_regionList.at(i)->show();
			m_regionList.at(i)->setZValue(1);
		}

		QList<QGraphicsItem *> li = m_scene->items();
		for (int i = 0; i < li.size(); ++i) {
			qDebug() << __PRETTY_FUNCTION__
				 << "Type:" << li.at(i)->type()
				 << "Obscured:" << li.at(i)->isObscured()
				 << "Scene Pos:" << li.at(i)->scenePos()
				 << "Bounding Rect:" << li.at(i)->boundingRect();
		}
	} else {
		m_view->setCursor(Qt::OpenHandCursor);
		m_view->setDragMode(QGraphicsView::ScrollHandDrag);
		m_view->setInteractive(false);

		for (int i = 0; i < m_regionList.size(); ++i) {
			//m_regionList.at(i)->hide();
			m_regionList.at(i)->setZValue(-1);
		}
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
void ImageViewManager::setFile(const QString &file)
{
	QPixmap pixmap;
	if (!pixmap.load(file)) {
		setImage(QPixmap());
	} else {
		setImage(pixmap);
	}

	//qDebug() << m_currentFile << "/" << m_fileList.count() << ":" << file;
}

// [SLOT public]
void ImageViewManager::setFiles(const QStringList &files)
{
	m_currentFile = 0;
	m_fileList = files;

	if (m_fileList.count() > 0) {
		setFile(m_fileList.at(m_currentFile));
	}
	updateButtons();
}

// [SLOT public]
void ImageViewManager::setImage(const QPixmap &pixmap)
{
	if (m_image) {
		delete m_image;
		m_scene->clear();
		m_view->reset();
		m_regionList.clear();
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

// public
QString ImageViewManager::currentFile() const
{
	if (m_fileList.count() > 0) {
		return m_fileList.at(m_currentFile);
	}

	return QString();
}

void ImageViewManager::showImageRegions(bool /*show*/)
{

}

// public
QList<QRectF> ImageViewManager::rectRegions() const
{
	QList<QRectF> rlist;
	for (int i = 0; i < m_regionList.count(); ++i) {
		rlist << m_regionList.at(i)->boundingRect();
	}
	return rlist;

#if 0
	QList<QRectF> rlist;
	QList<QGraphicsItem *> li = m_scene->items();
	for (int i = 0; i < li.size(); ++i) {
		if (QGraphicsRectItem::type() == li.at(i)->type()) {
			rlist << li.at(i)->boundingRect();
		}
	}
	return rlist;
#endif
}

// [SLOT public]
void ImageViewManager::addRectRegions(const QList<QRectF> regions)
{
	for (int i = 0; i < regions.count(); ++i) {
		QRectF ir = m_image->boundingRect();
		qreal x = regions.at(i).x() * ir.width();
		qreal y = regions.at(i).y() * ir.height();
		qreal w = regions.at(i).width() * ir.width();
		if (w < 2) {
			w = 2;
		}
		qreal h = regions.at(i).height() * ir.height();
		if (h < 2) {
			h = 2;
		}

		QRectF r(x, y, w, h);

		//qDebug() << __PRETTY_FUNCTION__ << "ir:" << ir << "r:" << r;
		//qDebug() << __PRETTY_FUNCTION__ << "Add:" << regions.at(i);
		addRectRegion(r);
	}
}

// [SLOT public]
void ImageViewManager::addRectRegion(const QRectF &region)
{
	//qDebug() << region << "Image:" << m_image->boundingRect();
#if 0
	QGraphicsRectItem *ir = m_scene->addRect(region);
	ir->setVisible(m_view->isInteractive());
	m_regionList.append(ir);
#else
	RegionGraphicsItem *ir = new RegionGraphicsItem(region);
	m_scene->addItem(ir);

//	ir->setVisible(m_view->isInteractive());
	m_regionList.append(static_cast<QGraphicsRectItem *>(ir));
#endif
}
