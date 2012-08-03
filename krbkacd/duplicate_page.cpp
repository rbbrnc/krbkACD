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
	qDebug() << __PRETTY_FUNCTION__;

	QList<QGraphicsItem *> itemList = ui->thumbView->items();
	for (int i = 0; i < itemList.size(); i++) {
		if (itemList.at(i)->isSelected()) {
		        QFileInfo file(itemList.at(i)->toolTip());
		        QString newName = QInputDialog::getText(this, tr("Rename"), tr("New filename: "), QLineEdit::Normal, file.fileName());
		        if (newName != file.fileName() && !newName.isEmpty()) {
		                if (QFile::rename(file.absoluteFilePath(), file.absolutePath() + "/" + newName))
					return;
				else
					QMessageBox::critical(this, tr("Error!"), tr("Renaming file %1 failed").arg(file.fileName()), QMessageBox::Ok);
			}
		}
	}
}

void DuplicatePage::deleteFile()
{
	qDebug() << __PRETTY_FUNCTION__;
	QList<QGraphicsItem *> itemList = ui->thumbView->items();
	QStringList selectedFiles;
	for (int i = 0; i < itemList.size(); i++) {
		if (itemList.at(i)->isSelected()) {
			selectedFiles << itemList.at(i)->toolTip();
		}
	}

	if (selectedFiles.isEmpty()) {
		QMessageBox::critical(this, tr("Delete File(s)"),
				            tr("No file(s) selected"), QMessageBox::Ok);
	} else {
		QString message = "Delete ";
		int count = selectedFiles.count();
		if (count == 1) {
			message += selectedFiles.first() + "?";
		} else {
			message += QString::number(count) + " files?";
		}
		if (QMessageBox::Yes == QMessageBox::warning(this, tr("Delete"), message, QMessageBox::Yes, QMessageBox::No)) {
			//delete files until filelist empty or error occured
			for (int i = 0; i < selectedFiles.size(); i++) {
				QFile file(selectedFiles.at(i));
				if (!file.remove()) {
					QMessageBox::critical(this,
						      tr("Error!"),
						      tr("Deleting file %1 failed").arg(file.fileName()),
						      QMessageBox::Ok);
					break;
				}
			}
		}
	}
}
