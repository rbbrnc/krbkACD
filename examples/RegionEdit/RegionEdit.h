#ifndef WIDGET_H
#define WIDGET_H

#include <QtGui>
#include <QList>

class Widget : public QWidget
{
	Q_OBJECT

	public:
		Widget(QWidget *parent = 0);
		~Widget();

	private slots:
		void ok();

	private:

		QVBoxLayout *m_mainLayout;
		QComboBox     *m_typeComboBox;
		QLineEdit     *m_posEdit;
		QLineEdit     *m_descEdit;
		QLineEdit     *m_nameEdit;

		QSize  m_dimension;
		QRectF m_area;
		int m_tagCount;
};

#endif // WIDGET_H
