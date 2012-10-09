#ifndef WIDGET_H
#define WIDGET_H

#include <QtGui>
#include <QList>


class PatternWidget : public QWidget
{
	Q_OBJECT

	public:

		enum PatternType {
			text = 0,
			date,
			count
		};

		PatternWidget(const QString &name, QWidget *parent = 0);
		~PatternWidget();

		QVariant value() const;
		enum PatternType type() const;

	signals:
		void deleteMe();

	private slots:
		void deleteMeClicked();
		void currentTypeChanged(const QString &text);
		void currentTypeChanged(int index);

	private:
		enum PatternType m_type;
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
		void ok();

	private:
		QList<PatternWidget *> m_patternList;

		QVBoxLayout *m_mainLayout;
		QVBoxLayout *m_patternLayout;
};

#endif // WIDGET_H
