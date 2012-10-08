#ifndef WIDGET_H
#define WIDGET_H

#include <QtGui>
#include <QList>


class PatternWidget : public QWidget
{
	Q_OBJECT

	public:
		PatternWidget(const QString &name, QWidget *parent = 0);
		~PatternWidget();

		QString value() const;

	signals:
		void deleteMe();

	private slots:
		void deleteMeClicked();
		void currentTypeChanged(const QString &text);
		void currentTypeChanged(int index);

	private:
		QToolButton   *m_deleteMeButton;
		QComboBox     *m_typeComboBox;
		QLineEdit     *m_textEdit;
		QDateTimeEdit *m_dateTimeEdit;
};

class Widget : public QWidget
{
	Q_OBJECT

	public:
		Widget(QWidget *parent = 0);
		~Widget();

	private slots:
		void addPattern();
		void removePattern();

	private:
		QList<PatternWidget *> m_patternList;

		QVBoxLayout *m_mainLayout;
		QVBoxLayout *m_patternLayout;
};

#endif // WIDGET_H
