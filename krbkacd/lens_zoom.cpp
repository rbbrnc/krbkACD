#include "lens_zoom.h"

#include <QDebug>

#if QT_VERSION < 0x0040500
#error You need Qt 4.5 or newer
#endif

// how long (milliseconds) the user need to hold (after a tap on the screen)
// before triggering the magnifying glass feature
// 701, a prime number, is the sum of 229, 233, 239
// (all three are also prime numbers, consecutive!)
#define HOLD_TIME 701

LensZoom::LensZoom(QWidget *parent)
{
        pressed = false;
        snapped = false;
        zoomed  = false;
        offset  = QPoint(0, 0);

	// Enable Mouse Move Events also without button press
	setMouseTracking(true);
}

bool LensZoom::setPixmap(QPixmap &pix)
{
	image = pix.toImage();
	largePixmap = pix;

	if (pix.isNull()) {
		smallPixmap.fill(Qt::transparent);
	} else {
	        smallPixmap = largePixmap.scaled(largePixmap.size() / 2);
	}

	qDebug() << __func__ << "LARGE W:" << largePixmap.width() << "H:" << largePixmap.height();
	qDebug() << __func__ << "SMALL W:" << smallPixmap.width() << "H:" << smallPixmap.height();
	return true;
}

void LensZoom::pan(const QPoint &delta)
{
	offset += delta;
	update();
}

void LensZoom::activateZoom()
{
	zoomed = true;
	tapTimer.stop();
	update();
}

void LensZoom::timerEvent(QTimerEvent *)
{
	if (!zoomed)
		activateZoom();
	update();
}

void LensZoom::mousePressEvent(QMouseEvent *event)
{
	if (event->buttons() != Qt::LeftButton)
		return;

	pressed = snapped = true;
	pressPos = dragPos = event->pos();
	tapTimer.stop();
	tapTimer.start(HOLD_TIME, this);
}

void LensZoom::mouseMoveEvent(QMouseEvent *event)
{
	if (!event->buttons()) {
		return;
	}

	qDebug() << "pos:" << event->pos();
	if (!zoomed) {
		if (!pressed || !snapped) {
			QPoint delta = event->pos() - pressPos;
			pressPos = event->pos();
			pan(delta);
			return;
		} else {
			const int threshold = 10;
			QPoint delta = event->pos() - pressPos;
			if (snapped) {
				snapped &= delta.x() < threshold;
				snapped &= delta.y() < threshold;
				snapped &= delta.x() > -threshold;
				snapped &= delta.y() > -threshold;
			}
			if (!snapped)
				tapTimer.stop();
		}
	} else {
		dragPos = event->pos();
		update();
	}

}

void LensZoom::mouseReleaseEvent(QMouseEvent *)
{
	zoomed = false;
	update();
}

void LensZoom::resizeEvent(QResizeEvent *)
{
	zoomed = false;
	update();
}

void LensZoom::paintEvent(QPaintEvent *)
{
	QPainter p(this);

	if (this->size().width() > smallPixmap.size().width()) {
		int x = (this->size().width() - smallPixmap.size().width())/2;
		offset.setX(x);
	}
	if (this->size().height() > smallPixmap.size().height()) {
		int y = (this->size().height() - smallPixmap.size().height())/2;
		offset.setY(y);
	}

	p.drawPixmap(offset, smallPixmap);
	p.end();

	if (zoomed) {
		int dim = qMin(width(), height());
		int magnifierSize = dim * 5 / 6;
		int radius = magnifierSize / 2;
		int ring = radius - 15;
		QSize box = QSize(magnifierSize, magnifierSize);

		// reupdate our mask
		if (maskPixmap.size() != box) {
			maskPixmap = QPixmap(box);
			maskPixmap.fill(Qt::transparent);

			QRadialGradient g;
			g.setCenter(radius, radius);
			g.setFocalPoint(radius, radius);
			g.setRadius(radius);
			g.setColorAt(1.0, QColor(64, 64, 64, 0));
			g.setColorAt(0.5, QColor(0, 0, 0, 255));

			QPainter mask(&maskPixmap);
			mask.setRenderHint(QPainter::Antialiasing);
			mask.setCompositionMode(QPainter::CompositionMode_Source);
			mask.setBrush(g);
			mask.setPen(Qt::NoPen);
			mask.drawRect(maskPixmap.rect());
			mask.setBrush(QColor(Qt::transparent));
			mask.drawEllipse(g.center(), ring, ring);
			mask.end();
		}

		QPoint center = dragPos - QPoint(0, radius);
		center = center + QPoint(0, radius / 2);
		QPoint corner = center - QPoint(radius, radius);

		QPoint xy = center * 2 - QPoint(radius, radius);

		// only set the dimension to the magnified portion
		if (zoomPixmap.size() != box)
			zoomPixmap = QPixmap(box);
		if (true) {
			zoomPixmap.fill(Qt::lightGray);
			QPainter p(&zoomPixmap);
			p.translate(-xy);
			p.drawPixmap(offset * 2, largePixmap);
			p.end();
		}

		QPainterPath clipPath;
		clipPath.addEllipse(center, ring, ring);

		QPainter p(this);
		p.setRenderHint(QPainter::Antialiasing);
		p.setClipPath(clipPath);
		p.drawPixmap(corner, zoomPixmap);
		p.setClipping(false);
		p.drawPixmap(corner, maskPixmap);
		p.setPen(Qt::gray);
		p.drawPath(clipPath);
	}
}

void LensZoom::keyPressEvent(QKeyEvent *event)
{
	if (!zoomed) {
		if (event->key() == Qt::Key_Left)
			offset += QPoint(20, 0);
		if (event->key() == Qt::Key_Right)
			offset += QPoint(-20, 0);
		if (event->key() == Qt::Key_Up)
			offset += QPoint(0, 20);
		if (event->key() == Qt::Key_Down)
			offset += QPoint(0, -20);
		if (event->key() == Qt::Key_Z || event->key() == Qt::Key_Select) {
			dragPos = QPoint(width() / 2, height() / 2);
			activateZoom();
		}
		update();
	} else {
		if (event->key() == Qt::Key_Z || event->key() == Qt::Key_Select) {
			zoomed = false;
			update();
		}
		QPoint delta(0, 0);
		if (event->key() == Qt::Key_Left)
			delta = QPoint(-15, 0);
		if (event->key() == Qt::Key_Right)
			delta = QPoint(15, 0);
		if (event->key() == Qt::Key_Up)
			delta = QPoint(0, -15);
		if (event->key() == Qt::Key_Down)
			delta = QPoint(0, 15);
		if (delta != QPoint(0, 0)) {
			dragPos += delta;
			update();
		}
	}
}

