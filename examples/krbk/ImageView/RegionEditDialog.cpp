#include "RegionEditDialog.h"
#include "ui_RegionEditDialog.h"

#include <QDebug>

RegionEditDialog::RegionEditDialog(RegionGraphicsItem *item, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::RegionEditDialog),
	m_item(item)
{
	ui->setupUi(this);

	if (item) {
		ui->nameLineEdit->setText(item->name());
		ui->descriptionTextEdit->setPlainText(item->description());
		ui->typeComboBox->setCurrentIndex(item->type());
	}
}

RegionEditDialog::~RegionEditDialog()
{
	delete ui;
}

void RegionEditDialog::accept()
{
	if (m_item) {
		m_item->setName(ui->nameLineEdit->text());
		m_item->setDescription(ui->descriptionTextEdit->toPlainText());
		m_item->setType(ui->typeComboBox->currentIndex());
	} else {
		reject();
	}

	QDialog::accept();
}

void RegionEditDialog::reject()
{
	emit rejected();
	QDialog::reject();
}

