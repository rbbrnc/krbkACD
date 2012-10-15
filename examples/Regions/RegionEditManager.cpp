#include <QtGui>

#include <QDebug>

#include "RegionEditManager.h"
#include "ImageGraphicsItem.h"
#include "ImageGraphicsView.h"
#include "QExiv2.h"

RegionEditManager::RegionEditManager(QWidget *parent)
	: QWidget(parent),
	  m_image(0),
	  m_metadata(0)
{
	m_scene = new QGraphicsScene(this);
	m_view  = new ImageGraphicsView(this);

	m_view->setScene(m_scene);
	m_view->setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(m_view);
	setLayout(layout);

	connect(m_scene, SIGNAL(changed(const QList<QRectF> &)), this, SLOT(sceneChanged(const QList<QRectF> &)));
}

RegionEditManager::~RegionEditManager()
{
	if (m_image) {
		delete m_image;
	}

	delete m_scene;
	delete m_view;
}

// [SLOT]
void RegionEditManager::sceneChanged(const QList<QRectF> &region)
{
	if (region.count() <= 0) {
		return;
	}

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

	m_image = new ImageGraphicsItem();
	m_image->setPixmap(pixmap);
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

