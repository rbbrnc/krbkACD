#include <QtGui>

#include <QDebug>

#include "ImageViewManager.h"
#include "ImageGraphicsItem.h"
#include "ImageGraphicsView.h"

ImageViewManager::ImageViewManager(QWidget *parent)
	: QWidget(parent),
	  m_image(0)
{
	m_scene = new QGraphicsScene(this);
	m_view  = new ImageGraphicsView(this);

	m_view->setScene(m_scene);

	QSize iconSize(32, 32);

	// Zoom In
	QToolButton *zoomInButton  = new QToolButton();
	zoomInButton->setIcon(QIcon(":/zoom_in.png"));
	zoomInButton->setIconSize(iconSize);

	// Zoom Out
	QToolButton *zoomOutButton   = new QToolButton();
	zoomOutButton->setIcon(QIcon(":/zoom_out.png"));
	zoomOutButton->setIconSize(iconSize);

	// Zoom to Fit
	QToolButton *zoomToFitButton = new QToolButton();
	zoomToFitButton->setIcon(QIcon(":/zoom_best_fit.png"));
	zoomToFitButton->setIconSize(iconSize);

	// Zoom 1:1
	QToolButton *zoom11Button    = new QToolButton();
	zoom11Button->setIcon(QIcon(":/zoom_original.png"));
	zoom11Button->setIconSize(iconSize);

	// Rotate CCW
	QToolButton *rotateCCWButton = new QToolButton();
	rotateCCWButton->setIcon(QIcon(":/rotate_ccw.png"));
	rotateCCWButton->setIconSize(iconSize);

	// Rotate CW
	QToolButton *rotateCWButton = new QToolButton();
	rotateCWButton->setIcon(QIcon(":/rotate_cw.png"));
	rotateCWButton->setIconSize(iconSize);

	// Reset View to original
	QToolButton *resetViewButton = new QToolButton();
	resetViewButton->setIcon(QIcon(":/original.png"));
	resetViewButton->setIconSize(iconSize);

	// Previous Image
	previousButton = new QToolButton();
	previousButton->setIcon(QIcon(":/previous.png"));
	previousButton->setIconSize(iconSize);

	// Next Image
	nextButton = new QToolButton();
	nextButton->setIcon(QIcon(":/next.png"));
	nextButton->setIconSize(iconSize);

	QHBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(zoomInButton);
	buttonLayout->addWidget(zoomOutButton);
	buttonLayout->addWidget(zoom11Button);
	buttonLayout->addWidget(zoomToFitButton);
	buttonLayout->addWidget(rotateCCWButton);
	buttonLayout->addWidget(rotateCWButton);
	buttonLayout->addWidget(resetViewButton);
	buttonLayout->addWidget(previousButton);
	buttonLayout->addWidget(nextButton);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(m_view);
	layout->addLayout(buttonLayout);
	setLayout(layout);

	connect(zoomInButton, SIGNAL(clicked()), m_view, SLOT(zoomIn()));
	connect(zoomOutButton, SIGNAL(clicked()), m_view, SLOT(zoomOut()));
	connect(zoom11Button, SIGNAL(clicked()), m_view, SLOT(zoom11()));
	connect(zoomToFitButton, SIGNAL(clicked()), m_view, SLOT(zoomToFit()));

	connect(rotateCCWButton, SIGNAL(clicked()), m_view, SLOT(rotateCCW()));
	connect(rotateCWButton, SIGNAL(clicked()), m_view, SLOT(rotateCW()));

	connect(resetViewButton, SIGNAL(clicked()), m_view, SLOT(reset()));

	connect(previousButton, SIGNAL(clicked()), this, SLOT(previous()));
	connect(nextButton, SIGNAL(clicked()), this, SLOT(next()));

	connect(m_scene, SIGNAL(changed(const QList<QRectF> &)), this, SLOT(sceneChanged(const QList<QRectF> &)));

	nextButton->setEnabled(false);
	previousButton->setEnabled(false);
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

// [SLOT]
void ImageViewManager::sceneChanged(const QList<QRectF> &region)
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
void ImageViewManager::setFile(const QString &file)
{
	QPixmap pixmap;
	if (!pixmap.load(file)) {
		return;
	}
	setImage(pixmap);

	//qDebug() << m_currentFile << "/" << m_fileList.count() << ":" << file;
}

void ImageViewManager::setFiles(const QStringList &files)
{
	m_currentFile = 0;
	m_fileList = files;

	if (m_fileList.count() > 0) {
		setFile(m_fileList.at(m_currentFile));
	}
	updateButtons();
}

void ImageViewManager::setImage(const QPixmap &pixmap)
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

void ImageViewManager::previous()
{
	if (m_currentFile > 0) {
		m_currentFile--;
		setFile(m_fileList.at(m_currentFile));
	}
	updateButtons();
}

void ImageViewManager::next()
{
	if (m_currentFile < m_fileList.count() - 1) {
		m_currentFile++;
		setFile(m_fileList.at(m_currentFile));
	}
	updateButtons();
}

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

