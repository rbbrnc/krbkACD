#include "RenameDialog.h"
#include "ui_RenameDialog.h"

#include <QDebug>
#include "PatternWidget.h"

RenameDialog::RenameDialog(const QString &fileName, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::RenameDialog)
{
	ui->setupUi(this);

	m_usePattern = false;
	ui->patternLayoutWidget->setVisible(m_usePattern);
	adjustSize();

	m_patternLayout = ui->patternLayout;
	m_originalName = fileName;
	ui->fileNameEdit->setText(fileName);
	ui->newName->setText("");

	QStringList patternTypeNames;
	patternTypeNames << "Text"
			 << "DateTime"
			 << "UUID";

	ui->patternComboBox->insertItems(0, patternTypeNames);

	connect(ui->addPatternButton, SIGNAL(clicked()), this, SLOT(addPattern()));
	connect(ui->patternCheckBox,  SIGNAL(toggled(bool)), this, SLOT(usePattern(bool)));
}

RenameDialog::~RenameDialog()
{
	delete ui;
}

void RenameDialog::usePattern(bool enable)
{
	if (enable) {
		ui->fileNameEdit->setText(m_originalName);
	}

	m_usePattern = enable;
	ui->fileNameEdit->setReadOnly(m_usePattern);
	ui->patternLayoutWidget->setVisible(m_usePattern);
	adjustSize();
}

void RenameDialog::addPattern()
{
	QString type = ui->patternComboBox->currentText();

	PatternWidget *pw = new PatternWidget(type, this);
	m_patternList.append(pw);
	m_patternLayout->addWidget(pw);
	connect(pw, SIGNAL(deleteMe()), this, SLOT(removePattern()));
	connect(pw, SIGNAL(valueChanged(const QString &)),
		this, SLOT(patternChanged(const QString &)));

	m_patternMap.insert(pw, pw->value().toString());

	// for pattern with predefined text.
	QString value = pw->value().toString();
	if (!value.isEmpty()) {
		ui->newName->setText(value);
	}
}

void RenameDialog::removePattern()
{
	PatternWidget *pw = static_cast<PatternWidget *>(sender());
	m_patternList.removeOne(pw);
	pw->hide();
	m_patternLayout->removeWidget(pw);
	m_patternMap.remove(pw);
	updateName();

	delete pw;
}

QString RenameDialog::newFileName()
{
	return m_newName;
}

void RenameDialog::accept()
{
	if (m_usePattern) {
		updateName();
	} else {
		m_newName = ui->fileNameEdit->text();
	}

	// Reject if the old name is equal to the new or new is empty.
	if ((m_newName == m_originalName) || (m_newName.isEmpty())) {
		emit rejected();
		QDialog::reject();
		return;
	}

	QDialog::accept();
}

void RenameDialog::patternChanged(const QString &pattern)
{
	PatternWidget *pw = static_cast<PatternWidget *>(sender());

	m_patternMap[pw] = pattern; //m_patternMap[pw] = pw->value().toString();
	updateName();
}

void RenameDialog::updateName()
{
	m_newName.clear();
	if (m_patternList.size() > 0) {
		for (int i = 0; i < m_patternList.size(); i++) {
			m_newName += m_patternMap[m_patternList.at(i)];
		}
	}

	ui->newName->setText(m_newName);
}

