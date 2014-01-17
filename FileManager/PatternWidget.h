#ifndef PATTERN_WIDGET_H
#define PATTERN_WIDGET_H

#include "qglobal.h"
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#else
#include <QtGui>
#endif

#include <QList>

class PatternWidget : public QWidget
{
	Q_OBJECT

	public:
		PatternWidget(const QString &name, const QVariant &value, QWidget *parent = 0);
		~PatternWidget();

		virtual QVariant value() const;

		static QStringList typeNames();

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
