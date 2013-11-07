#ifndef IMAGE_GRAPHICS_VIEW_H
#define IMAGE_GRAPHICS_VIEW_H

#include <QMouseEvent>
#include <QWheelEvent>
#include <QGraphicsView>

class ImageGraphicsView : public QGraphicsView
{
	Q_OBJECT

	public:
		ImageGraphicsView(QWidget *parent = 0);
		~ImageGraphicsView();

	signals:
		void newRectRegion(const QRectF &rect);

	public slots:
		void reset();

		void zoomIn();
		void zoomOut();
		void zoom11();
		void zoomToFit();

		void rotateCW();
		void rotateCCW();

	protected:
		// Holds the current centerpoint for the view,
		// used for panning and zooming
		QPointF m_currentCenterPoint;

		// From panning the view
		QPoint m_lastPanPoint;

		void setCenter(const QPointF& centerPoint);
		QPointF getCenter() const;

		virtual void wheelEvent(QWheelEvent *event);
		virtual void resizeEvent(QResizeEvent *event);
		virtual void mousePressEvent(QMouseEvent *event);
		virtual void mouseReleaseEvent(QMouseEvent *event);
		virtual void mouseMoveEvent(QMouseEvent *event);

	private:
		// Hold the curent rotation angle for restore
		// the current rotation after a Zoom 1:1.
		qreal m_angle;

		// Selection rectangle (interactive mode)
		QRectF m_selRect;
};

#endif
