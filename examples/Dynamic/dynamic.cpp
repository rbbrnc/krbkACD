#include "dynamic.h"

#include <QDebug>

PatternWidget::PatternWidget(const QString &name, QWidget *parent)
    : QWidget(parent)
{
	m_dateTimeEdit = 0;
	QHBoxLayout *mainLayout = new QHBoxLayout;

	m_deleteMeButton = new QToolButton();
	m_deleteMeButton->setIcon(QIcon(":/remove.png"));
	connect(m_deleteMeButton, SIGNAL(clicked()), this, SLOT(deleteMeClicked()));

	m_typeComboBox = new QComboBox();
	m_typeComboBox->insertItems(0, QStringList() << "Text" << "Date" << "Count");
	connect(m_typeComboBox, SIGNAL(currentIndexChanged(const QString &)),
		this, SLOT(currentTypeChanged(const QString &)));

//	connect(m_typeComboBox, SIGNAL(currentIndexChanged(int)),
//		this, SLOT(currentTypeChanged(int)));

	m_textEdit = new QLineEdit();

	mainLayout->addWidget(m_deleteMeButton);
	mainLayout->addWidget(m_typeComboBox);
	mainLayout->addWidget(m_textEdit);
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

QString PatternWidget::value() const
{
	return m_textEdit->text();
}

void PatternWidget::currentTypeChanged(const QString &text)
{
	qDebug() << __PRETTY_FUNCTION__ << "to" << text;
	if (text == "Text") {
	} else if (text == "Date") {

	} else {

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

	QPushButton *addButton = new QPushButton("Add", this);
	connect(addButton, SIGNAL(clicked()), this, SLOT(addPattern()));

	m_patternLayout = new QVBoxLayout;
	PatternWidget *pw = new PatternWidget(QString::number(m_patternList.count()), this);
	connect(pw, SIGNAL(deleteMe()), this, SLOT(removePattern()));
	m_patternList.append(pw);
	m_patternLayout->addWidget(pw);

	m_mainLayout->addWidget(addButton);
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
