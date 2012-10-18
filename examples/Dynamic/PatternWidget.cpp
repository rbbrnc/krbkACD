// Base Class for pattern widgets
#include "PatternWidget.h"

#include <uuid/uuid.h>
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


	QSizePolicy sp(QSizePolicy::Fixed, QSizePolicy::Fixed);
	sp.setHorizontalStretch(0);
	sp.setVerticalStretch(0);
	m_typeLabel = new QLabel(name);
	sp.setHeightForWidth(m_typeLabel->sizePolicy().hasHeightForWidth());
	m_typeLabel->setSizePolicy(sp);

	m_mainLayout->addWidget(m_typeLabel);

	qDebug() << __func__ << name;
	if (name == "UUID") {
		uuidPatternSetup();
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

void  PatternWidget::uuidPatternSetup()
{
	char hex_char[] = "0123456789ABCDEF";
	char uuidstr[sizeof(uuid_t) * 2 + 1];
	uuid_t uuid;
	uuid_generate(uuid);
	size_t byte_nbr;
	for (byte_nbr = 0; byte_nbr < sizeof(uuid_t); byte_nbr++) {
		uuidstr[byte_nbr * 2 + 0] = hex_char[uuid [byte_nbr] >> 4];
		uuidstr[byte_nbr * 2 + 1] = hex_char[uuid [byte_nbr] & 15];
	}

	QSizePolicy sp(QSizePolicy::Fixed, QSizePolicy::Expanding);
	sp.setHorizontalStretch(0);
	sp.setVerticalStretch(0);

	m_uuidLabel = new QLabel(QString(uuidstr));
	sp.setHeightForWidth(false/*m_uuidLabel->sizePolicy().hasHeightForWidth()*/);
	m_uuidLabel->setFrameShape(QFrame::Box);
	m_uuidLabel->setSizePolicy(sp);
	m_mainLayout->addWidget(m_uuidLabel);
}
