#include "fullscreen_page.h"
#include "ui_fullscreen_page.h"

FullScreenPage::FullScreenPage(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::FullScreenPage)
{
	ui->setupUi(this);
}

FullScreenPage::~FullScreenPage()
{
	delete ui;
}

void FullScreenPage::showEvent(QShowEvent *)
{
}

void FullScreenPage::hideEvent(QHideEvent *)
{

}

void FullScreenPage::setPixmap(QString file)
{
	//ui->pixFull->setPixmap(m_cPix);
}

void FullScreenPage::setPixmap(QPixmap pix)
{
	ui->pixFull->setPixmap(pix);
}
