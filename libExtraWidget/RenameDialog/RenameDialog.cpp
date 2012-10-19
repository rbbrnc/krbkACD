#include "RenameDialog.h"
#include "ui_RenameDialog.h"

//#include <QFileInfo>
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


	// Set file path, name, extension
	QFileInfo fi(fileName);
	m_originalPath = fi.absolutePath();
	m_originalName = fi.completeBaseName();
	m_originalExt  = "." + fi.suffix();

	m_newName = m_originalName;
	m_newExt = m_originalExt;

	ui->fileNameEdit->setText(m_newName + m_newExt);
	ui->newName->setText(m_newName + m_newExt);

	ui->fileExtensionEdit->setText(m_newExt);
	ui->fileExtensionEdit->setReadOnly(true);

	QStringList patternTypeNames;
	patternTypeNames << "Text"
			 << "DateTime"
			 << "UUID";

	ui->patternComboBox->insertItems(0, patternTypeNames);

	QStringList fileExtensionOptions;
	fileExtensionOptions << "Original"
		<< "Lower Case"
		<< "Upper Case"
		<< "Text";
	ui->fileExtensionComboBox->insertItems(0, fileExtensionOptions);
	connect(ui->fileExtensionComboBox, SIGNAL(currentIndexChanged(int)),
		this, SLOT(fileExtensionUpdate(int)));


	connect(ui->addPatternButton, SIGNAL(clicked()), this, SLOT(addPattern()));
	connect(ui->patternCheckBox,  SIGNAL(toggled(bool)), this, SLOT(usePattern(bool)));
	connect(ui->fileExtensionEdit, SIGNAL(textChanged(const QString &)),
		this, SLOT(fileExtensionChanged(const QString &)));

}

RenameDialog::~RenameDialog()
{
	delete ui;
}

// [public]
QString RenameDialog::newFileName()
{
	return m_originalPath + "/" + m_newName + m_newExt;
}

// [private]
void RenameDialog::updateName()
{
	m_newName.clear();
	if (m_patternList.size() > 0) {
		for (int i = 0; i < m_patternList.size(); i++) {
			m_newName += m_patternMap[m_patternList.at(i)];
		}
	}

	if (m_newName.isEmpty()) {
		ui->newName->setText(m_originalName + m_newExt);
	} else {
		ui->newName->setText(m_newName + m_newExt);
	}
}

// [SLOT public]
void RenameDialog::reject()
{
	emit rejected();
	QDialog::reject();
}

// [SLOT public]
void RenameDialog::accept()
{
	if (m_usePattern) {
		updateName();
		if (m_newName.isEmpty()) {
			if (m_newExt != m_originalExt) {
				// Only an extension modify
				m_newName = m_originalName;
			} else {
				reject();
				return;
			}
		}
	} else {
		m_newName = ui->fileNameEdit->text();
		m_newExt = "";
	}

	// Reject if the old name is equal to the new or new is empty.
	if ((m_newName + m_newExt == m_originalName + m_originalExt) || (m_newName.isEmpty())) {
		qDebug() << m_newName + m_newExt << " Rejected";
		reject();
		return;
	}

	QDialog::accept();
}
// [SLOT private]
void RenameDialog::usePattern(bool enable)
{
	if (enable) {
		ui->fileNameEdit->setText(m_originalName + m_originalExt);
	}

	m_usePattern = enable;
	ui->fileNameEdit->setReadOnly(m_usePattern);
	ui->patternLayoutWidget->setVisible(m_usePattern);
	adjustSize();
}

// [SLOT private]
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

	// For pattern with predefined text.
	QString value = pw->value().toString();
	if (!value.isEmpty()) {
		updateName();
	}
}

// [SLOT private]
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

// [SLOT private]
void RenameDialog::patternChanged(const QString &pattern)
{
	PatternWidget *pw = static_cast<PatternWidget *>(sender());
	m_patternMap[pw] = pattern; //m_patternMap[pw] = pw->value().toString();
	updateName();
}

// [SLOT private]
void RenameDialog::fileExtensionUpdate(int index)
{
	switch (index) {
	case 0:
		// Original
		m_newExt = m_originalExt;
		ui->fileExtensionEdit->setText(m_newExt);
		ui->fileExtensionEdit->setReadOnly(true);
		break;
	case 1:
		// Original lower case
		m_newExt = m_originalExt.toLower();
		ui->fileExtensionEdit->setText(m_newExt);
		ui->fileExtensionEdit->setReadOnly(true);
		break;
	case 2:
		// Original upper case
		m_newExt = m_originalExt.toUpper();
		ui->fileExtensionEdit->setText(m_newExt);
		ui->fileExtensionEdit->setReadOnly(true);

		break;
	case 3:
		// Free text
		ui->fileExtensionEdit->setReadOnly(false);
		break;
	default:
		break;
	}
	updateName();
}

// [SLOT private]
void RenameDialog::fileExtensionChanged(const QString &text)
{
	m_newExt = text;
	updateName();
}
