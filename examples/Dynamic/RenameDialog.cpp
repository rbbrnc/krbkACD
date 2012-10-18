#include "RenameDialog.h"
#include "ui_RenameDialog.h"

#include <QDebug>
#include "PatternWidget.h"

RenameDialog::RenameDialog(const QString &fileName, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::RenameDialog)
{
	ui->setupUi(this);
	m_patternLayout = ui->patternLayout;
	m_originalName = fileName;
	ui->originalFileName->setText(fileName);
	ui->newName->setText("");

	QStringList patternTypeNames;
	patternTypeNames << "Text"
			 << "Date"
			 << "UUID";

	ui->patternComboBox->insertItems(0, patternTypeNames);

	connect(ui->addPatternButton, SIGNAL(clicked()), this, SLOT(addPattern()));
}

RenameDialog::~RenameDialog()
{
	qDebug() << __PRETTY_FUNCTION__;
	delete ui;
}

void RenameDialog::addPattern()
{
	QString type = ui->patternComboBox->currentText();

	PatternWidget *pw = new PatternWidget(type, this);
	m_patternList.append(pw);
	m_patternLayout->addWidget(pw);
	connect(pw, SIGNAL(deleteMe()), this, SLOT(removePattern()));
}

void RenameDialog::removePattern()
{
	PatternWidget *pw = static_cast<PatternWidget *>(sender());
	m_patternList.removeOne(pw);
	pw->hide();
	m_patternLayout->removeWidget(pw);
	delete pw;
}

QString RenameDialog::newFileName()
{
	return ui->newName->text();
}

void RenameDialog::accept()
{
	// Reject if the old name is equal to the new.
	QString newName = ui->newName->text();
	if (newName == m_originalName) {
		emit rejected();
		QDialog::reject();
		return;
	}

	QDialog::accept();
}
