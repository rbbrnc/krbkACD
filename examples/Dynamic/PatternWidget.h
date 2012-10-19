#ifndef PATTERN_WIDGET_H
#define PATTERN_WIDGET_H

#include <QtGui>
#include <QList>

class PatternWidget : public QWidget
{
	Q_OBJECT

	public:
		PatternWidget(const QString &name, QWidget *parent = 0);
		~PatternWidget();

		virtual QVariant value() const;

	signals:
		void deleteMe();
		void valueChanged(const QString &);

	private slots:
		void deleteMeClicked();
		void textChangedNotify(const QString &);

	protected:
		QHBoxLayout *m_mainLayout;

	private:
		QLabel        *m_typeLabel;
		QToolButton   *m_deleteMeButton;

		QLineEdit     *m_textEdit;
		QDateTimeEdit *m_dateTimeEdit;
};

#endif
