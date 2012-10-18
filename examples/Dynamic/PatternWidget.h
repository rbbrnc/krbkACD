#ifndef PATTERN_WIDGET_H
#define PATTERN_WIDGET_H

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

		virtual QVariant value() const;
		virtual enum PatternType type() const;

	signals:
		void deleteMe();

	private slots:
		void deleteMeClicked();

	protected:
		QHBoxLayout *m_mainLayout;

	private:
		enum PatternType m_type;
		QLabel        *m_typeLabel;
		QToolButton   *m_deleteMeButton;

		QLineEdit     *m_textEdit;
		QLabel        *m_uuidLabel;
		QDateTimeEdit *m_dateTimeEdit;
};

#endif
