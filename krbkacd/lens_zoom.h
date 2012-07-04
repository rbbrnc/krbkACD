#ifndef LENS_ZOOM_H
#define LENS_ZOOM_H

#include <QtCore>
#include <QtGui>

class LensZoom : public QWidget
{
	Q_OBJECT

	public:
		LensZoom(QWidget *parent = 0);

		void pan(const QPoint &delta);
		void activateZoom();
		bool setPixmap(QPixmap &pix);

	protected:
		void timerEvent(QTimerEvent *);
		void mousePressEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *);
		void resizeEvent(QResizeEvent *);
		void paintEvent(QPaintEvent *);
		void keyPressEvent(QKeyEvent *event);

	private:
		QImage image;
		QPixmap smallPixmap;
		QPixmap largePixmap;
		QPoint offset;
		bool pressed;
		bool snapped;
		QPoint pressPos;
		QPoint dragPos;
		QBasicTimer tapTimer;
		bool zoomed;
		QPixmap zoomPixmap;
		QPixmap maskPixmap;
};

#endif // LENS_ZOOM_H
