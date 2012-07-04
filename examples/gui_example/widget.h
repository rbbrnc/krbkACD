#ifndef WIDGET_H
#define WIDGET_H

#include <QtGui/QWidget>
#include <QDesktopWidget>
#include <QApplication>
#include <QPixmap>
#include <QMouseEvent>
#include <QPoint>
#include <QRect>
#include <QSize>
#include <QPainter>
#include <QLabel>

#include "PTagWidget.h"
#include "QExiv2.h"

class Widget : public QWidget
{
	Q_OBJECT

	public:
		Widget(QWidget *parent = 0);
		Widget(QString file, QWidget *parent = 0);
		~Widget();

	protected:
		void resizeEvent(QResizeEvent *event);

	private:
		void mouseMoveEvent(QMouseEvent *);

	private:
		QLabel *label;
		QPixmap m_pic;

		QRect tagRect;
		QRectF tagRectF;

		PTagWidget *m_tag;
		QList<PTagWidget *> m_tagWidgetList;

		QExiv2 *exiv2;
		QList<PTag> m_tagList;
};

#endif // WIDGET_H
