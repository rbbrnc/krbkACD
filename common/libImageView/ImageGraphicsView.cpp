// cfr.
// http://www.qtcentre.org/wiki/index.php?title=QGraphicsView:_Smooth_Panning_and_Zooming
//
#include <QDebug>

#include "ImageGraphicsView.h"

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

// Return the current centerpoint for the view, used for zooming
QPointF ImageGraphicsView::GetCenter()
{
	return CurrentCenterPoint;
}

/**
 * Sets the current centerpoint.  Also updates the scene's center point.
 * Unlike centerOn, which has no way of getting the floating point center
 * back, SetCenter() stores the center point.  It also handles the special
 * sidebar case.  This function will claim the centerPoint to sceneRec ie.
 * the centerPoint must be within the sceneRec.
 */
void ImageGraphicsView::SetCenter(const QPointF &centerPoint)
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
		CurrentCenterPoint = centerPoint;
	} else {
		// We need to clamp or use the center of the screen
		if(visibleArea.contains(sceneBounds)) {
			// Use the center of scene ie. we can see the whole scene
			CurrentCenterPoint = sceneBounds.center();
		} else {
			CurrentCenterPoint = centerPoint;

			// We need to clamp the center. The centerPoint is too large
			if (centerPoint.x() > bounds.x() + bounds.width()) {
				CurrentCenterPoint.setX(bounds.x() + bounds.width());
			} else if(centerPoint.x() < bounds.x()) {
				CurrentCenterPoint.setX(bounds.x());
			}

			if(centerPoint.y() > bounds.y() + bounds.height()) {
				CurrentCenterPoint.setY(bounds.y() + bounds.height());
			} else if(centerPoint.y() < bounds.y()) {
				CurrentCenterPoint.setY(bounds.y());
			}
		}
	}

	// Update the scrollbars
	centerOn(CurrentCenterPoint);
}

// Zoom the view in and out.
void ImageGraphicsView::wheelEvent(QWheelEvent* event)
{
	// Get the position of the mouse before scaling, in scene coords
	QPointF pointBeforeScale(mapToScene(event->pos()));

	// Get the original screen centerpoint
	QPointF screenCenter = GetCenter();

	// Scale the view ie. do the zoom
	double scaleFactor = 1.15; //How fast we zoom
	if (event->delta() > 0) {
		// Zoom in
		scale(scaleFactor, scaleFactor);
	} else {
		// Zooming out
		scale(1.0 / scaleFactor, 1.0 / scaleFactor);
	}

	// Get the position after scaling, in scene coords
	QPointF pointAfterScale(mapToScene(event->pos()));

	// Get the offset of how the screen moved
	QPointF offset = pointBeforeScale - pointAfterScale;

	// Adjust to the new center for correct zooming
	QPointF newCenter = screenCenter + offset;
	SetCenter(newCenter);
}

// Handles when the mouse button is pressed
void ImageGraphicsView::mousePressEvent(QMouseEvent *event)
{
	if (isInteractive()) {
		// For region selection
		QPointF origin = mapToScene(event->pos());
		m_selRect.setTopLeft(origin);
		QGraphicsView::mousePressEvent(event);
	} else {
		// For panning the view
		LastPanPoint = event->pos();
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

		qDebug() << __PRETTY_FUNCTION__ << m_selRect;
		QGraphicsView::mouseReleaseEvent(event);
		emit newRectRegion(m_selRect);
	} else {
		setCursor(Qt::OpenHandCursor);
		LastPanPoint = QPoint();
	}
}

// Handles the mouse move event
void ImageGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
	if (isInteractive()) {
		QGraphicsView::mouseMoveEvent(event);
	} else {
		if (!LastPanPoint.isNull()) {
			// Get how much we panned
			QPointF delta = mapToScene(LastPanPoint) - mapToScene(event->pos());
			LastPanPoint = event->pos();

			// Update the center ie. do the pan
			SetCenter(GetCenter() + delta);
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
	SetCenter(visibleArea.center());

	//Call the subclass resize so the scrollbars are updated correctly
	QGraphicsView::resizeEvent(event);
}

void ImageGraphicsView::zoom(const enum ZoomType type)
{
	QTransform t;

	switch (type) {
	case ZoomIn:
		t = QTransform::fromScale(1.15, 1.15);
		setTransform(t, true);
		break;

	case ZoomOut:
		t = QTransform::fromScale(1.0/1.15, 1.0/1.15);
		setTransform(t, true);
		break;

	case ZoomOriginal:
		// Zoom 1:1
		t = QTransform::fromScale(1.0, 1.0);
		t.rotate(m_angle);
		setTransform(t, false);
		break;

	case ZoomToFit:
		fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
		break;

	default:
		break;
	}
}

void ImageGraphicsView::rotate(const enum RotationType type)
{
	QTransform t;

	switch (type) {
	case Rotation90CW:
		t.rotate(+90);
		m_angle += 90;
		if (m_angle >= 360) {
			m_angle = 0;
		}
		break;

	case Rotation90CCW:
		t.rotate(-90);
		m_angle -= 90;
		if (m_angle <= -360) {
			m_angle = 0;
		}
		break;

	default:
		return;
	}

	setTransform(t, true);
}

// [SLOT public]
void ImageGraphicsView::reset()
{
	m_angle = 0;
	resetTransform();
}
