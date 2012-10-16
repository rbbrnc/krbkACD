#include <QtGui>

#include <QDebug>

#include "RegionEditManager.h"
#include "ImageGraphicsItem.h"
#include "ImageRegionItem.h"
#include "ImageGraphicsView.h"
#include "QExiv2.h"

RegionEditManager::RegionEditManager(QWidget *parent)
	: QWidget(parent),
	  m_image(0),
	  m_metadata(0)
{
	m_scene = new QGraphicsScene(this);
	m_view  = new ImageGraphicsView(this);

	m_view->setCursor(Qt::ArrowCursor);
	m_view->setDragMode(QGraphicsView::RubberBandDrag);
	m_view->setInteractive(true);

	m_view->setScene(m_scene);
	m_view->setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(m_view);
	setLayout(layout);

	connect(m_scene, SIGNAL(changed(const QList<QRectF> &)), this, SLOT(sceneChanged(const QList<QRectF> &)));
	connect(m_view,  SIGNAL(newRectRegion(const QRectF &)),	this, SLOT(addRectRegion(const QRectF &)));
}

RegionEditManager::~RegionEditManager()
{

	for (int i = 0; i < m_regionList.size(); i++) {
		delete m_regionList.at(i);
	}

	if (m_image) {
		delete m_image;
	}


	//XXX delete all region items.!!!!
	delete m_scene;
	delete m_view;
}

// [SLOT]
void RegionEditManager::sceneChanged(const QList<QRectF> &region)
{
	if (region.count() <= 0) {
		return;
	}

//	qDebug() << __func__;


	// Don't resize the image if is totally contained
	// in the scene rect.
//	if (zoomToFitOption) {
		QSize viewSize = m_view->viewport()->size();
		if ((viewSize.width()  < region.at(0).toRect().width()) ||
		    (viewSize.height() < region.at(0).toRect().height())) {
			m_view->zoomToFit();
		}
//	}
}

// public slots:
void RegionEditManager::setFile(const QString &file)
{
	QPixmap pixmap;
	if (!pixmap.load(file)) {
		pixmap = QPixmap(1024, 1024);
		pixmap.fill(Qt::gray);
	}
	setImage(pixmap);
	m_imageSize = pixmap.size();;

	if (m_metadata) {
		delete m_metadata;
	}

	m_metadata = new QExiv2();
	if (m_metadata->load(file)) {
//		m_metadata = 0;
	}

	//qDebug() << m_currentFile << "/" << m_fileList.count() << ":" << file;
}

void RegionEditManager::setFiles(const QStringList &files)
{
	m_currentFile = 0;
	m_fileList = files;

	if (m_fileList.count() > 0) {
		setFile(m_fileList.at(m_currentFile));
	}
}

void RegionEditManager::setImage(const QPixmap &pixmap)
{
	if (m_image) {
		delete m_image;
		m_scene->clear();
		m_view->reset();
	}

	m_image = new ImageGraphicsItem(pixmap);
//	m_image->setPixmap(pixmap);
	m_scene->addItem(m_image);

	// QGraphicsScene's boundingRect will grow when items are added,
	// but doesn't shrink when items are removed.
	m_scene->setSceneRect(m_scene->itemsBoundingRect());
}

QString RegionEditManager::currentFile() const
{
	if (m_fileList.count() > 0) {
		return m_fileList.at(m_currentFile);
	}

	return QString();
}

void RegionEditManager::showImageRegions(bool /*show*/)
{

}

// [SLOT public]
void RegionEditManager::addRegion(const QRectF &region, const QString &text)
{
	QRectF rf(region.left() * m_imageSize.width(),
		  region.top() * m_imageSize.height(),
		  region.width() * m_imageSize.width(),
		  region.height() * m_imageSize.height());

	ImageRegionItem *ir = new ImageRegionItem(rf);
	ir->setText(text);
	m_scene->addItem(ir);
	m_regionList.append(ir);

	qDebug() << __PRETTY_FUNCTION__ << region << text;
}

// [SLOT public]
void RegionEditManager::addRectRegion(const QRectF &region)
{
//	qDebug() << region << "Image:" << m_image->boundingRect();
	//ImageRegionItem *ir = m_scene->addRect(region);

	ImageRegionItem *ir = new ImageRegionItem(region);
//	ir->setRect(region);
	m_scene->addItem(ir);
	m_regionList.append(ir);
}
