#ifndef IMAGE_GRAPHICS_VIEW_H
#define IMAGE_GRAPHICS_VIEW_H

#include <QMouseEvent>
#include <QWheelEvent>
#include <QGraphicsRectItem>
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
		QPointF CurrentCenterPoint;

		// From panning the view
		QPoint LastPanPoint;

		void SetCenter(const QPointF& centerPoint);
		QPointF GetCenter();

		virtual void wheelEvent(QWheelEvent *event);
		virtual void resizeEvent(QResizeEvent *event);
		virtual void mousePressEvent(QMouseEvent *event);
		virtual void mouseReleaseEvent(QMouseEvent *event);
		virtual void mouseMoveEvent(QMouseEvent *event);

	private:
		enum ZoomType {
			ZoomOriginal = 0,
			ZoomIn,
			ZoomOut,
			ZoomToFit
		};

		enum RotationType {
			Rotation90CW,
			Rotation90CCW,
		};

		void zoom(const enum ZoomType type);
		void rotate(const enum RotationType type);

	private:
		// Hold the curent rotation angle for restore
		// the current rotation after a Zoom 1:1.
		qreal m_angle;

		// Selection rectangle (interactive mode)
		QRectF m_selRect;
};

#endif
