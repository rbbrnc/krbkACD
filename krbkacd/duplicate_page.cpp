#include <QDebug>

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

void DuplicatePage::setMap(const QMap<QString, QStringList> &map)
{
	m_dupMap = map;
	m_current = 0;
	m_keyList = m_dupMap.keys();
	ui->thumbView->loadImages(m_dupMap.value(m_keyList.at(m_current)));
}

void DuplicatePage::on_backButton_clicked()
{
	emit changePage(WORK_PAGE);
}

void DuplicatePage::on_nextButton_clicked()
{
	if (m_current < m_keyList.size() - 1) {
		m_current++;
		ui->thumbView->loadImages(m_dupMap.value(m_keyList.at(m_current)));
	}
}

void DuplicatePage::on_prevButton_clicked()
{
	if (m_current > 0) {
		m_current--;
		ui->thumbView->loadImages(m_dupMap.value(m_keyList.at(m_current)));
	}
}
