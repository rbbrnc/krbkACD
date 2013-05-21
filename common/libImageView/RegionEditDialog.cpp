#include "RegionEditDialog.h"
#include "ui_RegionEditDialog.h"

#include <QDebug>

RegionEditDialog::RegionEditDialog(RegionGraphicsItem *item, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::RegionEditDialog),
	m_item(item)
{
	ui->setupUi(this);

	if (item) {
		//QPointF p = item->mapToScene(QPointF(item->x(), item->y()));
		QPointF p = item->scenePos();
		qDebug() << p;

		QString s = QString("x:%1, y:%2, w:%3, h:%4")
				.arg(p.x())
				.arg(p.y())
				.arg(item->rect().width())
				.arg(item->rect().height());

		ui->rectLabel->setText(s);
		ui->nameLineEdit->setText(item->name());
		ui->descriptionTextEdit->setPlainText(item->description());
	}
}

RegionEditDialog::~RegionEditDialog()
{
	delete ui;
}

void RegionEditDialog::accept()
{
	if (m_item) {
		m_item->setName(ui->nameLineEdit->text());
		m_item->setDescription(ui->descriptionTextEdit->toPlainText());
	} else {
		reject();
	}

	QDialog::accept();
}

void RegionEditDialog::reject()
{
	emit rejected();
	QDialog::reject();
}

