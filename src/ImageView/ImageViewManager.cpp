#include <QDebug>

#include "ImageViewManager.h"
#include "ImageGraphicsItem.h"
#include "RegionGraphicsItem.h"
#include "RegionEditDialog.h"
#include "ImageGraphicsView.h"
#include "QExiv2.h"

#ifdef ENABLE_OBJECT_DETECT_MODULE
#include "ObjectDetect.h"
#endif

ImageViewManager::ImageViewManager(QWidget *parent)
	: QWidget(parent),
      m_image(Q_NULLPTR),
      m_exiv2(Q_NULLPTR),
	  m_showRegions(false),
	  m_updateRegion(false)
{
	m_scene = new QGraphicsScene(this);
	m_view  = new ImageGraphicsView(this);

	m_view->setScene(m_scene);
	m_view->setBackgroundBrush(QBrush(Qt::gray, Qt::SolidPattern));

	QSize iconSize(32, 32);
	QHBoxLayout *buttonLayout = new QHBoxLayout;

	// Zoom In
	m_zoomInButton  = new QToolButton();
	m_zoomInButton->setIcon(QIcon(":/images/zoom_in.png"));
	m_zoomInButton->setIconSize(iconSize);
	buttonLayout->addWidget(m_zoomInButton);

	// Zoom Out
	m_zoomOutButton = new QToolButton();
	m_zoomOutButton->setIcon(QIcon(":/images/zoom_out.png"));
	m_zoomOutButton->setIconSize(iconSize);
	buttonLayout->addWidget(m_zoomOutButton);

	// Zoom to Fit
	m_zoomToFitButton = new QToolButton();
	m_zoomToFitButton->setIcon(QIcon(":/images/zoom_best_fit.png"));
	m_zoomToFitButton->setIconSize(iconSize);
	buttonLayout->addWidget(m_zoomToFitButton);

	// Zoom 1:1
	m_zoom11Button = new QToolButton();
	m_zoom11Button->setIcon(QIcon(":/images/zoom_original.png"));
	m_zoom11Button->setIconSize(iconSize);
	buttonLayout->addWidget(m_zoom11Button);

	// Rotate CCW
	m_rotateCCWButton = new QToolButton();
	m_rotateCCWButton->setIcon(QIcon(":/images/rotate_ccw.png"));
	m_rotateCCWButton->setIconSize(iconSize);
	buttonLayout->addWidget(m_rotateCCWButton);

	// Rotate CW
	m_rotateCWButton = new QToolButton();
	m_rotateCWButton->setIcon(QIcon(":/images/rotate_cw.png"));
	m_rotateCWButton->setIconSize(iconSize);
	buttonLayout->addWidget(m_rotateCWButton);

	// Reset View to original
	m_resetViewButton = new QToolButton();
	m_resetViewButton->setIcon(QIcon(":/images/original.png"));
	m_resetViewButton->setIconSize(iconSize);
	buttonLayout->addWidget(m_resetViewButton);

	// View Mode for Panning image or for region selection.
	m_modeButton = new QToolButton();
	m_modeButton->setCheckable(true);
	m_modeButton->setIcon(QIcon(":/images/select.png"));
	m_modeButton->setIconSize(iconSize);
	m_modeButton->setChecked(m_showRegions);
	buttonLayout->addWidget(m_modeButton);

#ifdef ENABLE_OBJECT_DETECT_MODULE
	// Detect Objects
	m_detectButton = new QToolButton();
	m_detectButton->setIcon(QIcon(":/images/user_silhouette.png"));
	m_detectButton->setIconSize(iconSize);
	m_detectButton->setEnabled(m_showRegions);
	buttonLayout->addWidget(m_detectButton);
#endif

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(m_view);
	layout->addLayout(buttonLayout);
	setLayout(layout);

    connect(m_zoomInButton,    &QToolButton::clicked, m_view, &ImageGraphicsView::zoomIn);
    connect(m_zoomOutButton,   &QToolButton::clicked, m_view, &ImageGraphicsView::zoomOut);
    connect(m_zoom11Button,    &QToolButton::clicked, m_view, &ImageGraphicsView::zoom11);
    connect(m_zoomToFitButton, &QToolButton::clicked, m_view, &ImageGraphicsView::zoomToFit);
    connect(m_rotateCCWButton, &QToolButton::clicked, m_view, &ImageGraphicsView::rotateCCW);
    connect(m_rotateCWButton,  &QToolButton::clicked, m_view, &ImageGraphicsView::rotateCW);
    connect(m_resetViewButton, &QToolButton::clicked, m_view, &ImageGraphicsView::reset);
    connect(m_modeButton,      &QToolButton::toggled, this,   &ImageViewManager::enableRegionSelection);

#ifdef ENABLE_OBJECT_DETECT_MODULE
    connect(m_detectButton, &QToolButton::clicked, this, &ImageViewManager::onDetectObjects);
#endif
    connect(m_scene, &QGraphicsScene::changed, this, &ImageViewManager::sceneChanged);
    connect(m_view, &ImageGraphicsView::newRectRegion, this, &ImageViewManager::addRegion);
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

void ImageViewManager::loadMetadata()
{
	if (m_exiv2) {
		delete m_exiv2;
        m_exiv2 = Q_NULLPTR;
	}

	m_exiv2 = new QExiv2();
	if (m_exiv2->load(m_file)) {
		if (m_exiv2->xmpHasRegionTags()) {
			// Get XMP Image Regions
			MwgRegionList rl = m_exiv2->xmpMwgRegionList();
			for (int i = 0; i < rl.count(); i++) {
				insertRegion(rl.at(i).stAreaBoundingRectF(),
					     rl.at(i).name(),
					     rl.at(i).description(),
					     rl.at(i).type());
			}
		}
	}
}

void ImageViewManager::saveMetadata()
{
	if (m_updateRegion && m_exiv2) {
		int rc = QMessageBox::question(this,
			 tr("Metadata Changed"), tr("Save Metadata for file:\n%1").arg(m_file),
			 QMessageBox::Ok | QMessageBox::Cancel,
			 QMessageBox::Cancel);

		if (QMessageBox::Ok == rc) {
			if (!m_regions.isEmpty()) {
				MwgRegionList regs;
				foreach(RegionGraphicsItem *item, m_regions) {
					MwgRegion r;
					r.setRegion(item->boundingRect(), QSizeF(m_image->pixmap().size()), false);
					r.setDescription(item->description());
					r.setName(item->name());
					r.setType(static_cast<MwgRs::Type>(item->type()));
					regs.append(r);
				}
				m_exiv2->xmpSetMwgRegionList(regs);
			}
			m_exiv2->save();
		}
		m_updateRegion = false;
	}
}

// [SLOT private]
void ImageViewManager::sceneChanged(const QList<QRectF> &region)
{
	if (region.count() <= 0) {
		return;
	}

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
void ImageViewManager::setImage(const QString &fileName, bool withMetadata)
{
    m_file = fileName;

    if (m_image) {
        delete m_image;
        m_scene->clear();
        m_view->reset();
        m_regions.clear();
    }

    m_image = new ImageGraphicsItem();

    QImageReader ir(fileName);

    if (!ir.canRead()) {
        m_scene->addItem(new QGraphicsTextItem("Not a Pixmap"));
    } else {
        if (ir.supportsAnimation() && (ir.imageCount() > 1)) {
            m_image->setMovie(fileName);
        } else {
            m_image->setPixmap(QPixmap(fileName));
            if (withMetadata) {
                loadMetadata();
            }
        }
    }

    m_scene->addItem(m_image);

    // QGraphicsScene's boundingRect will grow when items are added,
    // but doesn't shrink when items are removed.
    m_scene->setSceneRect(m_scene->itemsBoundingRect());
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

#ifdef ENABLE_OBJECT_DETECT_MODULE
	m_detectButton->setEnabled(enable);
#endif

	showRegions(enable);
}

void ImageViewManager::insertRegion(const QRectF &rect, const QString &name,
				    const QString &desc, int type)
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

	RegionGraphicsItem *ir = new RegionGraphicsItem(rect);

	ir->setType(type);
	if (!name.isNull()) {
		ir->setName(name);
	}

	if (!desc.isNull()) {
		ir->setDescription(desc);
	}

    connect(ir, &RegionGraphicsItem::removeRequest, this, &ImageViewManager::removeRegion);
    connect(ir, &RegionGraphicsItem::editRequest,   this, &ImageViewManager::editRegion);

	m_scene->addItem(ir);
	m_regions.insert(ir);

	if (!m_showRegions) {
		ir->setZValue(-1);
	}
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

    int rc = QMessageBox::question(this, "Remove Region", msg, QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel);
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

#ifdef ENABLE_OBJECT_DETECT_MODULE
void ImageViewManager::onDetectObjects()
{
	ObjectDetect *o = new ObjectDetect();
	o->setSource(m_file.toLatin1().data());
	int n = o->detect();

	if (n > 0) {
		QList<QRectF> regions = o->objects();
		for (int i = 0; i < regions.count(); i++) {
			addRegion(regions.at(i));
		}
	} else {
		QMessageBox::warning(this, "Detect Faces", "No Faces Detected!");
	}

	delete o;
}
#endif

