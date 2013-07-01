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
	m_view->setBackgroundBrush(QBrush(Qt::gray, Qt::SolidPattern));

	QSize iconSize(32, 32);

	// Zoom In
	m_zoomInButton  = new QToolButton();
	m_zoomInButton->setIcon(QIcon(":/images/zoom_in.png"));
	m_zoomInButton->setIconSize(iconSize);

	// Zoom Out
	m_zoomOutButton = new QToolButton();
	m_zoomOutButton->setIcon(QIcon(":/images/zoom_out.png"));
	m_zoomOutButton->setIconSize(iconSize);

	// Zoom to Fit
	m_zoomToFitButton = new QToolButton();
	m_zoomToFitButton->setIcon(QIcon(":/images/zoom_best_fit.png"));
	m_zoomToFitButton->setIconSize(iconSize);

	// Zoom 1:1
	m_zoom11Button = new QToolButton();
	m_zoom11Button->setIcon(QIcon(":/images/zoom_original.png"));
	m_zoom11Button->setIconSize(iconSize);

	// Rotate CCW
	m_rotateCCWButton = new QToolButton();
	m_rotateCCWButton->setIcon(QIcon(":/images/rotate_ccw.png"));
	m_rotateCCWButton->setIconSize(iconSize);

	// Rotate CW
	m_rotateCWButton = new QToolButton();
	m_rotateCWButton->setIcon(QIcon(":/images/rotate_cw.png"));
	m_rotateCWButton->setIconSize(iconSize);

	// Reset View to original
	m_resetViewButton = new QToolButton();
	m_resetViewButton->setIcon(QIcon(":/images/original.png"));
	m_resetViewButton->setIconSize(iconSize);

	// View Mode for Panning image or for region selection.
	m_modeButton = new QToolButton();
	m_modeButton->setCheckable(true);
	m_modeButton->setIcon(QIcon(":/images/select.png"));
	m_modeButton->setIconSize(iconSize);
	m_modeButton->setChecked(m_showRegions);

	// Detect Objects
	m_detectButton = new QToolButton();
	m_detectButton->setIcon(QIcon(":/images/user_silhouette.png"));
	m_detectButton->setIconSize(iconSize);
	m_detectButton->setEnabled(m_showRegions);

	// Previous Image
	QToolButton *previousButton = new QToolButton();
	previousButton->setIcon(QIcon(":/images/previous.png"));
	previousButton->setIconSize(iconSize);

	// Next Image
	QToolButton *nextButton = new QToolButton();
	nextButton->setIcon(QIcon(":/images/next.png"));
	nextButton->setIconSize(iconSize);

	QHBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(m_zoomInButton);
	buttonLayout->addWidget(m_zoomOutButton);
	buttonLayout->addWidget(m_zoom11Button);
	buttonLayout->addWidget(m_zoomToFitButton);
	buttonLayout->addWidget(m_rotateCCWButton);
	buttonLayout->addWidget(m_rotateCWButton);
	buttonLayout->addWidget(m_resetViewButton);
	buttonLayout->addWidget(m_modeButton);
	buttonLayout->addWidget(m_detectButton);
	buttonLayout->addWidget(previousButton);
	buttonLayout->addWidget(nextButton);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(m_view);
	layout->addLayout(buttonLayout);
	setLayout(layout);

	connect(m_zoomInButton,    SIGNAL(clicked()), m_view, SLOT(zoomIn()));
	connect(m_zoomOutButton,   SIGNAL(clicked()), m_view, SLOT(zoomOut()));
	connect(m_zoom11Button,    SIGNAL(clicked()), m_view, SLOT(zoom11()));
	connect(m_zoomToFitButton, SIGNAL(clicked()), m_view, SLOT(zoomToFit()));

	connect(m_rotateCCWButton, SIGNAL(clicked()), m_view, SLOT(rotateCCW()));
	connect(m_rotateCWButton,  SIGNAL(clicked()), m_view, SLOT(rotateCW()));

	connect(m_resetViewButton, SIGNAL(clicked()), m_view, SLOT(reset()));

	connect(m_modeButton, SIGNAL(toggled(bool)), this, SLOT(enableRegionSelection(bool)));
	connect(m_detectButton, SIGNAL(clicked()), this, SLOT(onDetectObjects()));

	connect(previousButton, SIGNAL(clicked()), this, SLOT(previous()));
	connect(nextButton,     SIGNAL(clicked()), this, SLOT(next()));

	connect(m_scene, SIGNAL(changed(const QList<QRectF> &)), this, SLOT(sceneChanged(const QList<QRectF> &)));
	connect(m_view,  SIGNAL(newRectRegion(const QRectF &)), this, SLOT(addRegion(const QRectF &)));
}

ImageViewManager::~ImageViewManager()
{
	if (m_exiv2) {
		delete m_exiv2;
	}

	if (m_image) {
		delete m_image;
	}

	delete m_scene;
	delete m_view;
}

void ImageViewManager::saveMetadata()
{
	if (m_updateRegion && m_exiv2) {
		int rc = QMessageBox::question(this,
			 tr("Metadata Changed"), tr("Save Metadata ?"),
			 QMessageBox::Ok | QMessageBox::Cancel,
			 QMessageBox::Cancel);

		if (QMessageBox::Ok == rc) {
			qDebug() << __PRETTY_FUNCTION__ << "SAVE";
			if (!m_regions.isEmpty()) {
				MwgRegionList regs;
				foreach(RegionGraphicsItem *item, m_regions) {
					MwgRegion r;
					r.setRegion(item->boundingRect(), QSizeF(m_image->pixmap().size()), false);
					r.setDescription(item->description());
					r.setName(item->name());
					//qDebug() << "Save:\n" << r;
					regs.append(r);
				}
				m_exiv2->xmpSetMwgRegionList(regs);
			}
			m_exiv2->save();
		} else {
			qDebug() << __PRETTY_FUNCTION__ << "CANCEL_SAVE";
		}
	} else {
		qDebug() << __PRETTY_FUNCTION__ << "NOTHING_SAVE";
	}
}

// [SLOT public]
void ImageViewManager::previous()
{
	saveMetadata();
	emit requestPreviousFile();
}

// [SLOT public]
void ImageViewManager::next()
{
	saveMetadata();
	emit requestNextFile();
}

void ImageViewManager::onDetectObjects()
{
	emit requestDetectObjects();
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
void ImageViewManager::setImage(const QString &fileName, bool loadMetadata)
{
	QPixmap pixmap;
	if (!pixmap.load(fileName)) {
		setImage(QPixmap());
		return;
	} else {
		setImage(pixmap);
	}

        if (loadMetadata) {
		if (m_exiv2) {
			delete m_exiv2;
			m_exiv2 = 0;
		}

                m_exiv2 = new QExiv2();
                if (m_exiv2->load(fileName)) {
                        if (m_exiv2->xmpHasRegionTags()) {
                                // Get XMP Image Regions
                                MwgRegionList rl = m_exiv2->xmpMwgRegionList();
                                for (int i = 0; i < rl.count(); i++) {
					insertRegion(rl.at(i).stAreaBoundingRectF(),
						     rl.at(i).name(),
						     rl.at(i).description());

                                        qDebug() << __PRETTY_FUNCTION__
                                                 << rl.at(i).stAreaBoundingRectF()
                                                 << rl.at(i).stArea()
                                                 << rl.at(i).name()
                                                 << rl.at(i).description();
                                }
                        }
                }
        }
}

// [SLOT public]
void ImageViewManager::setImage(const QPixmap &pixmap)
{
	if (m_image) {
		delete m_image;
		m_scene->clear();
		m_view->reset();
		m_regions.clear();
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

// [SLOT private]
// checked = true -> Set image panning mode
// checked = true -> Set region select mode.
void ImageViewManager::enableRegionSelection(bool enable)
{
	if (enable) {
		m_view->setCursor(Qt::ArrowCursor);
		m_view->setDragMode(QGraphicsView::RubberBandDrag);
	} else {
		m_view->setCursor(Qt::OpenHandCursor);
		m_view->setDragMode(QGraphicsView::ScrollHandDrag);
	}

	m_view->setInteractive(enable);
	m_detectButton->setEnabled(enable);
	showRegions(enable);
}

void ImageViewManager::insertRegion(const QRectF &rect, const QString &name, const QString &desc)
{
	if (!m_image) {
		qWarning() << __PRETTY_FUNCTION__ << "Invalid Image" << rect;
		return;
	}

	if (!rect.isValid()) {
		qWarning() << __PRETTY_FUNCTION__ << "Invalid Rect:" << rect;
		return;
	}

	// Check if the rect is totally internal to the pixmap
	// to avoid select a region outside the pixmap
	if (!m_image->boundingRect().contains(rect)) {
		qWarning() << __PRETTY_FUNCTION__ << "Region not fully bounded on image";
		return;
	}
	qDebug() << __PRETTY_FUNCTION__ << rect << name << desc;

	RegionGraphicsItem *ir = new RegionGraphicsItem(rect);

	if (!name.isNull()) {
		ir->setName(name);
	}

	if (!desc.isNull()) {
		ir->setDescription(desc);
	}

	connect(ir, SIGNAL(removeRequest()), this, SLOT(removeRegion()));
	connect(ir, SIGNAL(editRequest()),   this, SLOT(editRegion()));

	m_scene->addItem(ir);
	m_regions.insert(ir);

	if (!m_showRegions) {
		ir->setZValue(-1);
	}
#if 0
	QSize imageSize;
	imageSize.setWidth(m_image->boundingRect().width());
	imageSize.setHeight(m_image->boundingRect().height());
	m_updateRegion = true;
#endif
}

// [SLOT public]
// Called for new created regions with rubberband selection rect
void ImageViewManager::addRegion(const QRectF &rect)
{
	insertRegion(rect, "", "");
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

	int rc = QMessageBox::question(this,
			 "Remove Region", msg,
			 QMessageBox::Ok | QMessageBox::Cancel,
			 QMessageBox::Cancel);

	if (QMessageBox::Cancel == rc) {
		return;
	}

	m_regions.remove(ri);
	m_scene->removeItem(dynamic_cast<QGraphicsItem *>(ri));
	m_updateRegion = true;
}


// [SLOT private]
void ImageViewManager::editRegion()
{
	RegionGraphicsItem *ri = dynamic_cast<RegionGraphicsItem *>(sender());
	RegionEditDialog dlg(ri, this);
	if (QDialog::Accepted == dlg.exec()) {
		//qDebug() << "Name:" << ri->name() << "Desc:" << ri->description();
		m_updateRegion = true;
	}
}

// [SLOT public]
void ImageViewManager::showRegions(bool show)
{
	if (m_showRegions == show) {
		return;
	}

	if (!m_regions.isEmpty()) {
		foreach(RegionGraphicsItem *item, m_regions) {
			item->setZValue((show) ? 1 : -1);
		}
	}

	m_showRegions = show;
}

