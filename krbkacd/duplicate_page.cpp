#include <QDebug>

#include "duplicate_page.h"
#include "ui_duplicate_page.h"

#include "pages.h"
#include "file_utils.h"

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

void DuplicatePage::contextMenuEvent(QGraphicsSceneContextMenuEvent *e)
{
	qDebug() << __PRETTY_FUNCTION__;

        QMenu *menu = new QMenu;
        QAction *action;
	action = menu->addAction("DPRotate CW");
	connect(menu, SIGNAL(triggered()), this, SLOT(rotateCW()));

        action = menu->addAction("DPRotate CCW");
	connect(menu, SIGNAL(triggered()), this, SLOT(rotateCCW()));

        menu->popup(e->screenPos());
}

/* SLOT [public]
 *
 * Rename selected file
 */
void DuplicatePage::renameFile()
{
	QList<QGraphicsItem *> itemList = ui->thumbView->items();
	for (int i = 0; i < itemList.size(); i++) {
		if (itemList.at(i)->isSelected()) {
		        QFileInfo file(itemList.at(i)->toolTip());
			::renameFile(file.absoluteFilePath(), this);
		}
	}
}

void DuplicatePage::deleteFile()
{
	QList<QGraphicsItem *> itemList = ui->thumbView->items();
	QStringList selectedFiles;
	for (int i = 0; i < itemList.size(); i++) {
		if (itemList.at(i)->isSelected()) {
			selectedFiles << itemList.at(i)->toolTip();
		}
	}

	::deleteFiles(selectedFiles, this);
}
