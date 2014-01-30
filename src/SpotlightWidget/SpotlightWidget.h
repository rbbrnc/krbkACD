#ifndef SPOTLIGHT_WIDGET_H
#define SPOTLIGHT_WIDGET_H

#include <QLineEdit>
#include <QToolButton>
#include <QLabel>

class SpotlightWidget : public QLineEdit
{
	Q_OBJECT

	public:
		SpotlightWidget(QWidget *parent = 0);

	protected:
		void resizeEvent(QResizeEvent *);

	private slots:
		void updateCloseButton(const QString &text);

	private:
		QToolButton *clearButton;
		QLabel *glass;
};

#endif // SPOTLIGHT_WIDGET_H

