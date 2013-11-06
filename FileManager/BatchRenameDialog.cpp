#include "BatchRenameDialog.h"
#include "ui_BatchRenameDialog.h"

#include <QDebug>
#include "PatternWidget.h"

BatchRenameDialog::BatchRenameDialog(const QStringList &files, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::BatchRenameDialog)
{
	ui->setupUi(this);

	ui->fileExtensionEdit->hide();
	m_patternLayout = ui->patternLayout;

	// Set file path, names and extensions
	QFileInfo fi(files.at(0));
	m_originalPath = fi.absolutePath();

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

	ui->patternComboBox->insertItems(0, PatternWidget::typeNames());

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
		sl << m_newName.at(i) + m_newExt.at(i);
	}
	return sl;
}
#if 0
// [public]
QStringList BatchRenameDialog::newFilePaths()
{
	QStringList sl;

	for (int i = 0; i < m_newName.count(); i++) {
		sl << m_originalPath + "/" + m_newName.at(i) + m_newExt.at(i);
	}
	return sl;
}
#endif

// [private]
void BatchRenameDialog::updateNames()
{
	ui->newNamesListWidget->clear();

	int nc = m_newName.count();
	if (nc >= 10000) {
		nc = 8;
	} else {
		nc = 4;
	}

	for (int i = 0; i < m_newName.count(); i++) {
		if (m_patternList.size() > 0) {
			QString newName;
			for (int j = 0; j < m_patternList.size(); j++) {
				newName += m_patternMap[m_patternList.at(j)];
			}
			// Add count pattern!
			//newName += QString("_") + QString::number(i);
			newName += QString("_%1").arg(i, nc, 10, QChar('0'));
			m_newName.replace(i, newName);
		} else {
			m_newName.replace(i, m_originalName.at(i));
		}

		ui->newNamesListWidget->addItem(m_newName.at(i) + m_newExt.at(i));
	}
}

// [SLOT public]
void BatchRenameDialog::reject()
{
	emit rejected();
	QDialog::reject();
}

// [SLOT public]
void BatchRenameDialog::accept()
{
	updateNames();

	// Lists consistency check!
	if (m_newName.count() != m_originalName.count()) {
		reject();
		return;
	}

	for (int i = 0; i < m_newName.count(); i++) {
		if (m_newName.at(i).isEmpty()) {
			if (m_newExt.at(i) != m_originalExt.at(i)) {
				// Only an extension modify
				m_newName.replace(i, m_originalName.at(i));
			} else {
				qDebug() << m_newName.at(i) + m_newExt.at(i) << " Rejected (Empty Name)";
				reject();
				return;
			}
		}
		// Reject if the old name is equal to the new or new is empty.
		if (m_newName.at(i) + m_newExt.at(i) == m_originalName.at(i) + m_originalExt.at(i)) {
			qDebug() << i << ":"
				 << m_originalName.at(i) + m_originalExt.at(i)
				 << "-->"
				 << m_newName.at(i) + m_newExt.at(i)
				 << " Rejected (same names)";
			reject();
			return;
		}
	}

	QDialog::accept();
}

// [SLOT private]
void BatchRenameDialog::addPattern()
{
	QString type = ui->patternComboBox->currentText();

	PatternWidget *pw = new PatternWidget(type, QVariant(), this);
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
		ui->fileExtensionEdit->hide();
		break;
	case 1:
		// Original lower case
		for (int i = 0; i < m_newExt.count(); i++) {
			m_newExt.replace(i, m_originalExt.at(i).toLower());
		}
		ui->fileExtensionEdit->setText("");
		ui->fileExtensionEdit->setReadOnly(true);
		ui->fileExtensionEdit->hide();
		break;
	case 2:
		// Original upper case
		for (int i = 0; i < m_newExt.count(); i++) {
			m_newExt.replace(i, m_originalExt.at(i).toUpper());
		}
		ui->fileExtensionEdit->setText("");
		ui->fileExtensionEdit->setReadOnly(true);
		ui->fileExtensionEdit->hide();

		break;
	case 3:
		// Free text
		ui->fileExtensionEdit->setReadOnly(false);
		ui->fileExtensionEdit->show();
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

