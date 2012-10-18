#include "dynamic.h"

#include <QDebug>

PatternWidget::PatternWidget(const QString &name, QWidget *parent)
    : QWidget(parent)
{

	m_type = PatternWidget::text;
	m_dateTimeEdit = 0;
	QHBoxLayout *mainLayout = new QHBoxLayout;

	m_deleteMeButton = new QToolButton();
	m_deleteMeButton->setIcon(QIcon(":/remove.png"));
	connect(m_deleteMeButton, SIGNAL(clicked()), this, SLOT(deleteMeClicked()));

	QStringList patternTypeNames;
	patternTypeNames << "Text"
			 << "Date"
			 << "UUID";

	m_typeComboBox = new QComboBox();
	m_typeComboBox->insertItems(0, patternTypeNames);
	connect(m_typeComboBox, SIGNAL(currentIndexChanged(const QString &)),
		this, SLOT(currentTypeChanged(const QString &)));

//	connect(m_typeComboBox, SIGNAL(currentIndexChanged(int)),
//		this, SLOT(currentTypeChanged(int)));

	m_textEdit = new QLineEdit();

	mainLayout->addWidget(m_typeComboBox);
	mainLayout->addWidget(m_textEdit);
	mainLayout->addWidget(m_deleteMeButton);
	setLayout(mainLayout);
}

PatternWidget::~PatternWidget()
{
	//qDebug() << __PRETTY_FUNCTION__;
	delete m_deleteMeButton;
	delete m_typeComboBox;
	if (m_textEdit) {
		delete m_textEdit;
	}
	if (m_dateTimeEdit) {
		delete m_dateTimeEdit;
	}
}

QVariant PatternWidget::value() const
{
	return m_textEdit->text();
}

enum PatternWidget::PatternType PatternWidget::type() const
{
	return m_type;
}

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

void PatternWidget::deleteMeClicked()
{
	emit deleteMe();
}

//======================

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{

	m_mainLayout = new QVBoxLayout;

	QHBoxLayout *buttonLayout = new QHBoxLayout;
	QPushButton *addButton = new QPushButton("Add", this);
	connect(addButton, SIGNAL(clicked()), this, SLOT(addPattern()));

	QPushButton *okButton = new QPushButton("Ok", this);
	connect(okButton, SIGNAL(clicked()), this, SLOT(ok()));

	buttonLayout->addWidget(addButton);
	buttonLayout->addWidget(okButton);

	m_patternLayout = new QVBoxLayout;
	PatternWidget *pw = new PatternWidget(QString::number(m_patternList.count()), this);
	connect(pw, SIGNAL(deleteMe()), this, SLOT(removePattern()));
	m_patternList.append(pw);
	m_patternLayout->addWidget(pw);

	m_mainLayout->addLayout(buttonLayout);
	m_mainLayout->addLayout(m_patternLayout);
	setLayout(m_mainLayout);
}

Widget::~Widget()
{
	for (int i = 0; i < m_patternList.count(); i++) {
		delete m_patternList.at(i);
	}
	delete m_patternLayout;
	delete m_mainLayout;
}

void Widget::ok()
{
	QString s;
	for (int i = 0; i < m_patternList.count(); i++) {
		PatternWidget *pw = m_patternList.at(i);
		switch (pw->type()) {
		case PatternWidget::text:
			s += pw->value().toString();
			break;
		case PatternWidget::date:
			s += "DATE";
			break;
		case PatternWidget::count:
			s += "COUNT";
			break;
		}
	}

	qDebug() << s;
}

void Widget::addPattern()
{
	PatternWidget *pw = new PatternWidget(QString::number(m_patternList.count()), this);
	m_patternList.append(pw);
	m_patternLayout->addWidget(pw);
	connect(pw, SIGNAL(deleteMe()), this, SLOT(removePattern()));
}

void Widget::removePattern()
{
	PatternWidget *pw = static_cast<PatternWidget *>(sender());
	m_patternList.removeOne(pw);
	pw->hide();
	m_patternLayout->removeWidget(pw);
	delete pw;
}
