#include "PatternWidget.h"

#include <QUuid>
#include <QDebug>

PatternWidget::PatternWidget(const QString &name, QWidget *parent)
	: QWidget(parent),
	  m_textEdit(0),
	  m_dateTimeEdit(0)

{
	m_dateTimeEdit = 0;
	m_mainLayout = new QHBoxLayout;

	m_deleteMeButton = new QToolButton();
	m_deleteMeButton->setIcon(QIcon(":/images/remove.png"));
	connect(m_deleteMeButton, SIGNAL(clicked()), this, SLOT(deleteMeClicked()));


	QSizePolicy sp(QSizePolicy::Fixed, QSizePolicy::Fixed);
	sp.setHorizontalStretch(0);
	sp.setVerticalStretch(0);
	m_typeLabel = new QLabel(name);
	sp.setHeightForWidth(m_typeLabel->sizePolicy().hasHeightForWidth());
	m_typeLabel->setSizePolicy(sp);

	m_mainLayout->addWidget(m_typeLabel);

	m_textEdit = new QLineEdit();

	if (name == "UUID") {
		QString uuid = QUuid::createUuid().toString();
		uuid.remove('{');
		uuid.remove('}');
		uuid.remove('-');
		uuid = uuid.toUpper();
		m_textEdit->setText(uuid);
		m_textEdit->setReadOnly(true);
	} else if (name == "DateTime") {
		m_textEdit->setText("YYYYMMDDTHHMMSS");
	} else {
	}

	m_mainLayout->addWidget(m_textEdit);
	m_mainLayout->addWidget(m_deleteMeButton);
	setLayout(m_mainLayout);

	connect(m_textEdit, SIGNAL(textChanged(const QString &)),
		this, SLOT(textChangedNotify(const QString &)));
}

PatternWidget::~PatternWidget()
{
	delete m_typeLabel;
	delete m_deleteMeButton;
	if (m_textEdit) {
		delete m_textEdit;
	}

	if (m_dateTimeEdit) {
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

void PatternWidget::textChangedNotify(const QString &text)
{
	emit valueChanged(text);
}
