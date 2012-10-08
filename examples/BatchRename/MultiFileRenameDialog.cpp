#include "MultiFileRenameDialog.h"
#include "ui_MultiFileRenameDialog.h"

#include <QtGui>
#include <QFileInfo>
#include <QDebug>

MultiFileRenameDialog::MultiFileRenameDialog(const QStringList &files, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::MultiFileRenameDialog)
{
	ui->setupUi(this);

	m_oldList = files;
	m_newList = files;
	for (int i = 0; i < m_oldList.count(); i++) {
		ui->oldNamesListWidget->addItem(m_oldList.at(i));
		//ui->newNamesListWidget->addItem(files.at(i));
	}
}

MultiFileRenameDialog::~MultiFileRenameDialog()
{
	delete ui;
}

void MultiFileRenameDialog::accept()
{
	if (renameFiles()) {
		QDialog::accept();
	} else {
		//ui->errorLabel->setText("Error");
	}
}

/* Rename files */
bool MultiFileRenameDialog::renameFiles()
{
	QFileInfo fi;
	bool rc = true;
	for (int i = 0; i < m_oldList.count(); i++) {
		fi = QFileInfo(m_oldList.at(i));

		if (m_newList.at(i).isEmpty()) {
			rc = false;
			continue;
		}

		QString newName = fi.absolutePath() + "/" + m_newList.at(i);
		qDebug() << m_oldList.at(i) << "-->" << newName;

//		if (!QFile::rename(fi.absoluteFilePath(), newName)) {
//			rc = false;
//		}
	}

	return rc;
}

void MultiFileRenameDialog::on_addToolButton_clicked()
{
	QLabel *p = new QLabel("NEW", ui->scrollAreaWidgetContents);
	//ui->patternLayout->addWidget(p);

	
}
