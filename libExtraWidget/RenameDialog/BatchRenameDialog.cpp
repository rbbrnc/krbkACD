#include "BatchRenameDialog.h"
#include "ui_BatchRenameDialog.h"

#include <QtGui>
#include <QDebug>

#include "PatternWidget.h"

BatchRenameDialog::BatchRenameDialog(const QStringList &files, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::BatchRenameDialog)
{
	ui->setupUi(this);

	m_patternLayout = ui->patternLayout;

	// Set file path, names and extensions
	QFileInfo fi(files.at(0));
	m_originalPath = fi.absolutePath();

	QString oldName;
	QString newName;
	for (int i = 0; i < files.count(); i++) {
		fi = QFileInfo(files.at(i));
		m_originalName << fi.completeBaseName();

		if (fi.suffix().isEmpty()) {
			m_originalExt << "";
		} else {
			m_originalExt << "." + fi.suffix();
		}

		m_newName << m_originalName.at(i);
		m_newExt  << m_originalExt.at(i);

		ui->oldNamesListWidget->addItem(m_originalName.at(i) + m_originalExt.at(i));
		ui->newNamesListWidget->addItem(m_newName.at(i) + m_newExt.at(i));
	}

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
	connect(ui->fileExtensionEdit, SIGNAL(textChanged(const QString &)),
		this, SLOT(fileExtensionChanged(const QString &)));

	connect(ui->addPatternButton, SIGNAL(clicked()), this, SLOT(addPattern()));
}

BatchRenameDialog::~BatchRenameDialog()
{
	delete ui;
}

// [public]
QStringList BatchRenameDialog::newFileNames()
{
	QStringList sl;

	for (int i = 0; i < m_newName.count(); i++) {
		sl << m_originalPath + "/" + m_newName.at(i) + m_newExt.at(i);
	}
	return sl;
}

void BatchRenameDialog::reject()
{
	QDialog::reject();
}

void BatchRenameDialog::accept()
{
	QDialog::accept();
}

// [private]
void BatchRenameDialog::updateNames()
{
	ui->newNamesListWidget->clear();

	for (int i = 0; i < m_newName.count(); i++) {
		if (m_patternList.size() > 0) {
			QString newName;
			for (int j = 0; j < m_patternList.size(); j++) {
				newName += m_patternMap[m_patternList.at(j)];
			}
			// Add count pattern!
			newName += QString("_") + QString::number(i);
			m_newName.replace(i, newName);
		} else {
			m_newName.replace(i, m_originalName.at(i));
		}

		ui->newNamesListWidget->addItem(m_newName.at(i) + m_newExt.at(i));
	}
}

// [SLOT private]
void BatchRenameDialog::fileExtensionUpdate(int index)
{
	switch (index) {
	case 0:
		// Original
		for (int i = 0; i < m_newExt.count(); i++) {
			m_newExt.replace(i, m_originalExt.at(i));
		}
		ui->fileExtensionEdit->setText("");
		ui->fileExtensionEdit->setReadOnly(true);
		break;
	case 1:
		// Original lower case
		for (int i = 0; i < m_newExt.count(); i++) {
			m_newExt.replace(i, m_originalExt.at(i).toLower());
		}
		ui->fileExtensionEdit->setText("");
		ui->fileExtensionEdit->setReadOnly(true);
		break;
	case 2:
		// Original upper case
		for (int i = 0; i < m_newExt.count(); i++) {
			m_newExt.replace(i, m_originalExt.at(i).toUpper());
		}
		ui->fileExtensionEdit->setText("");
		ui->fileExtensionEdit->setReadOnly(true);

		break;
	case 3:
		// Free text
		ui->fileExtensionEdit->setReadOnly(false);
		break;
	default:
		break;
	}
	updateNames();
}

// [SLOT private]
void BatchRenameDialog::fileExtensionChanged(const QString &text)
{
	for (int i = 0; i < m_newExt.count(); i++) {
		m_newExt.replace(i, text);
	}
	updateNames();
}

// [SLOT private]
void BatchRenameDialog::addPattern()
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
		updateNames();
	}
}

// [SLOT private]
void BatchRenameDialog::removePattern()
{
	PatternWidget *pw = static_cast<PatternWidget *>(sender());
	m_patternList.removeOne(pw);
	pw->hide();
	m_patternLayout->removeWidget(pw);
	m_patternMap.remove(pw);
	updateNames();

	delete pw;
}

// [SLOT private]
void BatchRenameDialog::patternChanged(const QString &pattern)
{
	PatternWidget *pw = static_cast<PatternWidget *>(sender());
	m_patternMap[pw] = pattern; //m_patternMap[pw] = pw->value().toString();
	updateNames();
}

