#include "RegionEditDialog.h"
#include "ui_RegionEditDialog.h"

RegionEditDialog::RegionEditDialog(XmpRegion region, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::RegionEditDialog)
{
	ui->setupUi(this);

	ui->srcLabel->setText(region.name());
	ui->destLabel->setText(region.description());
}

RegionEditDialog::~RegionEditDialog()
{
	delete ui;
}
