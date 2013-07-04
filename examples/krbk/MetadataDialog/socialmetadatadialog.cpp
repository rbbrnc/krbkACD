#include "socialmetadatadialog.h"
#include "ui_socialmetadatadialog.h"

//#include "QExiv2.h"

SocialMetadataDialog::SocialMetadataDialog(const QStringList &files, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SocialMetadataDialog)
{
	ui->setupUi(this);

/*
	QExiv2 *e = new QExiv2();
	if (!e->load(file)) {
		delete e;
		return false;
	}
*/

//	ui->label->setTextFormat(Qt::RichText);
//	ui->label->setTextInteractionFlags(Qt::TextBrowserInteraction);
//	ui->label->setOpenExternalLinks(true);

//	if (m_fileList.count() == 1) {
	ui->label->setText("<a href=\"http://www.google.com/\">Google</a>");
}

SocialMetadataDialog::~SocialMetadataDialog()
{
	delete ui;
}
