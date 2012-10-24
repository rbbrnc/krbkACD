#include "CopyMoveDialog.h"
#include "ui_CopyMoveDialog.h"

CopyMoveDialog::CopyMoveDialog(const QString &file, const QString &toPath, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CopyMoveDialog)
{
	ui->setupUi(this);

	ui->srcLabel->setText(file);
	ui->destLabel->setText(toPath);
}

CopyMoveDialog::CopyMoveDialog(const QStringList &files, const QString &toPath, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CopyMoveDialog)
{
	ui->setupUi(this);
	ui->srcLabel->setText("*");
	ui->destLabel->setText(toPath);
}

CopyMoveDialog::~CopyMoveDialog()
{
	delete ui;
}
