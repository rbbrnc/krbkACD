#include "duplicate_page.h"
#include "ui_duplicate_page.h"

#include "pages.h"

DuplicatePage::DuplicatePage(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::DuplicatePage)
{
	ui->setupUi(this);
}

DuplicatePage::~DuplicatePage()
{
	delete ui;
}

void DuplicatePage::on_backButton_clicked()
{
	emit changePage(WORK_PAGE);
}

void DuplicatePage::on_nextButton_clicked()
{

}

void DuplicatePage::on_prevButton_clicked()
{

}
