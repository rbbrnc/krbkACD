#ifndef P_TAG_WIDGET_H
#define P_TAG_WIDGET_H

#include <QWidget>

#include "PTag.h"

class PTagWidget : public QWidget
{
	public:
		PTagWidget(QWidget *parent = 0);
		PTagWidget(const PTag &tag, QSize imgSize, QWidget *parent = 0);
		~PTagWidget();

		void setFrameSize(int w, int h);
		void setPTag(const PTag &tag);
		QRect tagRect() const;

	protected:
		void paintEvent(QPaintEvent *pe);

	private:
		void updateRect();

	private:
		QSize  m_imgSize;	// Whole frame
		QRect  m_rect;		// Actual region rect (screen coordinates)
		QRect  m_textRect;	// Rect for description Tag

		PTag	m_ptag;
};

#endif // P_TAG_WIDGET_H
