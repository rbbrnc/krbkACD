#include "widget.h"

#include <QLabel>
#include <QPixmap>

#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
	label = new QLabel(this);

	this->setMouseTracking(true);
	label->setMouseTracking(true);

	label->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

	int w = this->width();
	int h = this->height();

	resize(w, h);
}

Widget::Widget(QString file, QWidget *parent)
{
	label = new QLabel(this);

	this->setMouseTracking(true);
	label->setMouseTracking(true);

	if (m_pic.load(file)) {
		label->setPixmap(m_pic);
		int w = m_pic.width();
		int h = m_pic.height();
		resize(w, h);

		exiv2 = new QExiv2();
		if (exiv2->load(file)) {
			m_tagList = exiv2->xmpPTags();
			for (int i = 0; i < m_tagList.size(); i++) {
				m_tag = new PTagWidget(m_tagList.at(i), QSize(w, h), this);
				m_tagWidgetList.append(m_tag);
				//m_tagList.at(i).debug();
			}
		}
	}
}

Widget::~Widget()
{
	for (int i = 0; i < m_tagWidgetList.size(); i++) {
		delete m_tagWidgetList.at(i);
	 }

//	for (int i = 0; i < m_tagList.size(); i++) {
//		delete m_tagList.at(i);
//	 }
}

void Widget::resizeEvent(QResizeEvent *event)
{
	int w = event->size().width();
	int h = event->size().height();

	if (!m_pic.isNull()) {
		m_pic = m_pic.scaled(w, h, Qt::KeepAspectRatio);
		label->setPixmap(m_pic);
	}

	for (int i = 0; i < m_tagWidgetList.size(); i++) {
		m_tagWidgetList.at(i)->setFrameSize(w, h);
	 }
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
	int x = event->x();
	int y = event->y();

	for (int i = 0; i < m_tagWidgetList.size(); i++) {
		PTagWidget *t = m_tagWidgetList.at(i);
		QRect tagRect = t->tagRect();

		if (tagRect.contains(x, y) && t->isHidden()) {
			t->show();
		} else {
			if (t->isVisible()) {
				t->hide();
			}
		}
	 }
}

