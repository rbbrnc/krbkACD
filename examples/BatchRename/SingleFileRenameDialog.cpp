#include "SingleFileRenameDialog.h"
#include "ui_SingleFileRenameDialog.h"

#include <QFileInfo>
#include <QDebug>

SingleFileRenameDialog::SingleFileRenameDialog(const QString &file, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SingleFileRenameDialog)
{
	ui->setupUi(this);
	m_oldName = file;
	ui->originalName->setText(m_oldName);
	ui->newName->setText(m_oldName);
}

SingleFileRenameDialog::~SingleFileRenameDialog()
{
	delete ui;
}

void SingleFileRenameDialog::accept()
{
	if (renameFile()) {
		QDialog::accept();
	} else {
		ui->errorLabel->setText("Error");
	}
}

/* Rename file */
bool SingleFileRenameDialog::renameFile()
{
	QFileInfo fi(m_oldName);
	QString newName = ui->newName->text();

	qDebug() << m_oldName << "-->" << newName;

	if (newName.isEmpty()) {
		return false;
	}

	if (newName == m_oldName) {
		return true;
	}

	if (QFile::rename(fi.absoluteFilePath(), fi.absolutePath() + "/" + newName)) {
		return true;
	}

	return false;
}
