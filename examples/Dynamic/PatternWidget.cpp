// Base Class for pattern widgets
#include "PatternWidget.h"

#include <QDebug>

PatternWidget::PatternWidget(const QString &name, QWidget *parent)
	: QWidget(parent),
	  m_textEdit(0),
	  m_uuidLabel(0),
	  m_dateTimeEdit(0)

{
	m_type = PatternWidget::text;
	m_dateTimeEdit = 0;
	m_mainLayout = new QHBoxLayout;

	m_deleteMeButton = new QToolButton();
	m_deleteMeButton->setIcon(QIcon(":/images/remove.png"));
	connect(m_deleteMeButton, SIGNAL(clicked()), this, SLOT(deleteMeClicked()));

	m_typeLabel = new QLabel(name);
	m_mainLayout->addWidget(m_typeLabel);

	if (name == "UUID") {
		m_uuidLabel = new QLabel("uuid_");
	} else {
		m_textEdit = new QLineEdit();
		m_mainLayout->addWidget(m_textEdit);
	}

	m_mainLayout->addWidget(m_deleteMeButton);
	setLayout(m_mainLayout);
}

PatternWidget::~PatternWidget()
{
	//qDebug() << __PRETTY_FUNCTION__;
	delete m_typeLabel;
	delete m_deleteMeButton;
	if (m_textEdit) {
		delete m_textEdit;
	}

	if (m_dateTimeEdit) {
		delete m_dateTimeEdit;
	}

	if (m_uuidLabel) {
		delete m_uuidLabel;
	}

	delete m_mainLayout;
}

void PatternWidget::deleteMeClicked()
{
	emit deleteMe();
}

QVariant PatternWidget::value() const
{
	return m_textEdit->text();
}

enum PatternWidget::PatternType PatternWidget::type() const
{
	return m_type;
}

#if 0
void PatternWidget::currentTypeChanged(const QString &text)
{
	qDebug() << __PRETTY_FUNCTION__ << "to" << text;
	if (text == "Text") {
		m_type = PatternWidget::text;
	} else if (text == "Date") {
		m_type = PatternWidget::date;


	} else if (text == "Count") {
		m_type = PatternWidget::count;
	} else {
		// default
		m_type = PatternWidget::text;
	}
}

void PatternWidget::currentTypeChanged(int index)
{
	qDebug() << __PRETTY_FUNCTION__;
}
#endif

