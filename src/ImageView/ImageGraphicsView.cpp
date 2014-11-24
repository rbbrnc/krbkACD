// cfr.
// http://www.qtcentre.org/wiki/index.php?title=QGraphicsView:_Smooth_Panning_and_Zooming
//

#include "ImageGraphicsView.h"

// Zoom scale factors (aka. How fast we zoom)
#define Z_IN   1.15
#define Z_OUT  (1.0 / Z_IN)

ImageGraphicsView::ImageGraphicsView(QWidget *parent)
	: QGraphicsView(parent),
	m_angle(0)
{
	setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

	// Default scrollHandDrag non interactive.
	setCursor(Qt::OpenHandCursor);
	setDragMode(QGraphicsView::ScrollHandDrag);
	setInteractive(false);

	setOptimizationFlags(QGraphicsView::DontSavePainterState);
	setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
}

ImageGraphicsView::~ImageGraphicsView()
{
}

/**
 * Sets the current centerpoint.  Also updates the scene's center point.
 * Unlike centerOn, which has no way of getting the floating point center
 * back, setCenter() stores the center point.  It also handles the special
 * sidebar case.  This function will claim the centerPoint to sceneRec ie.
 * the centerPoint must be within the sceneRec.
 */
void ImageGraphicsView::setCenter(const QPointF &centerPoint)
{
	// Get the rectangle of the visible area in scene coords
	QRectF visibleArea = mapToScene(rect()).boundingRect();

        // Get the scene area
        QRectF sceneBounds = sceneRect();

	double boundX = visibleArea.width() / 2.0;
	double boundY = visibleArea.height() / 2.0;
	double boundWidth = sceneBounds.width() - 2.0 * boundX;
	double boundHeight = sceneBounds.height() - 2.0 * boundY;

	// The max boundary that the centerPoint can be to
	QRectF bounds(boundX, boundY, boundWidth, boundHeight);

	if(bounds.contains(centerPoint)) {
		// We are within the bounds
		m_currentCenterPoint = centerPoint;
	} else {
		// We need to clamp or use the center of the screen
		if(visibleArea.contains(sceneBounds)) {
			// Use the center of scene ie. we can see the whole scene
			m_currentCenterPoint = sceneBounds.center();
		} else {
			m_currentCenterPoint = centerPoint;

			// We need to clamp the center. The centerPoint is too large
			if (centerPoint.x() > bounds.x() + bounds.width()) {
				m_currentCenterPoint.setX(bounds.x() + bounds.width());
			} else if(centerPoint.x() < bounds.x()) {
				m_currentCenterPoint.setX(bounds.x());
			}

			if(centerPoint.y() > bounds.y() + bounds.height()) {
				m_currentCenterPoint.setY(bounds.y() + bounds.height());
			} else if(centerPoint.y() < bounds.y()) {
				m_currentCenterPoint.setY(bounds.y());
			}
		}
	}

	// Update the scrollbars
	centerOn(m_currentCenterPoint);
}

// Zoom the view in and out.
void ImageGraphicsView::wheelEvent(QWheelEvent* event)
{
	// Get the position of the mouse before scaling, in scene coords
	QPointF pointBeforeScale(mapToScene(event->pos()));

	// Get the original screen centerpoint
    QPointF screenCenter = m_currentCenterPoint;

	// Scale the view (ie. do the zoom)
	if (event->delta() > 0) {
		scale(Z_IN, Z_IN);	// Zoom in
	} else {
        scale(Z_OUT, Z_OUT); // Zooming out
	}

	// Get the position after scaling, in scene coords
	QPointF pointAfterScale(mapToScene(event->pos()));

	// Get the offset of how the screen moved
	QPointF offset = pointBeforeScale - pointAfterScale;

	// Adjust to the new center for correct zooming
	QPointF newCenter = screenCenter + offset;
	setCenter(newCenter);
}

// Handles when the mouse button is pressed
void ImageGraphicsView::mousePressEvent(QMouseEvent *event)
{
	if (isInteractive()) {
		// For region selection
        m_selRect.setTopLeft(mapToScene(event->pos()));
		QGraphicsView::mousePressEvent(event);
	} else {
		// For panning the view
		m_lastPanPoint = event->pos();
		setCursor(Qt::ClosedHandCursor);
	}
}

// Handles when the mouse button is released
void ImageGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
	if (isInteractive()) {
		QPointF origin = m_selRect.topLeft();
		QPointF end    = mapToScene(event->pos());

		if (end.x() < origin.x()) {
			m_selRect.setX(end.x());
			m_selRect.setWidth(origin.x() - end.x());
		} else {
			m_selRect.setWidth(end.x() - origin.x());
		}

		if (end.y() < origin.y()) {
			m_selRect.setY(end.y());
			m_selRect.setHeight(origin.y() - end.y());
		} else {
			m_selRect.setHeight(end.y() - origin.y());
		}

		QGraphicsView::mouseReleaseEvent(event);
		emit newRectRegion(m_selRect);
	} else {
		setCursor(Qt::OpenHandCursor);
		m_lastPanPoint = QPoint();
	}
}

// Handles the mouse move event
void ImageGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
	if (isInteractive()) {
		QGraphicsView::mouseMoveEvent(event);
	} else {
		if (!m_lastPanPoint.isNull()) {
			// Get how much we panned
			QPointF delta = mapToScene(m_lastPanPoint) - mapToScene(event->pos());
			m_lastPanPoint = event->pos();

			// Update the center ie. do the pan
            setCenter(m_currentCenterPoint + delta);
		}
	}
}

/**
 * Need to update the center so there is no jolt in the
 * interaction after resizing the widget.
 */
void ImageGraphicsView::resizeEvent(QResizeEvent* event)
{
	// Get the rectangle of the visible area in scene coords
	QRectF visibleArea = mapToScene(rect()).boundingRect();
	setCenter(visibleArea.center());

	//Call the subclass resize so the scrollbars are updated correctly
	QGraphicsView::resizeEvent(event);
}

// [SLOT public]
void ImageGraphicsView::reset()
{
	m_angle = 0;
	resetTransform();
}

// [SLOT public]
void ImageGraphicsView::zoomIn()
{
	setTransform(QTransform::fromScale(Z_IN, Z_IN), true);
}

// [SLOT public]
void ImageGraphicsView::zoomOut()
{
	setTransform(QTransform::fromScale(Z_OUT, Z_OUT), true);
}

// Zoom 1:1 [SLOT public]
void ImageGraphicsView::zoom11()
{
	setTransform(QTransform::fromScale(1.0, 1.0).rotate(m_angle), false);
}

// [SLOT public]
void ImageGraphicsView::zoomToFit()
{
	fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
}

// [SLOT public]
void ImageGraphicsView::rotateCW()
{
	m_angle += 90;

	if (m_angle >= 360) {
		m_angle = 0;
	}

	setTransform(QTransform::fromScale(1.0, 1.0).rotate(90), true);
}

// [SLOT public]
void ImageGraphicsView::rotateCCW()
{
	m_angle -= 90;
	if (m_angle <= -360) {
		m_angle = 0;
	}

	setTransform(QTransform::fromScale(1.0, 1.0).rotate(-90), true);
}

