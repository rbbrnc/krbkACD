#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QMessageBox>
#include <QDebug>

#include "QExiv2.h"

MainWindow::MainWindow(const QString file, QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	m_xmpUpdate = false;
	ui->saveButton->setEnabled(m_xmpUpdate);

	//XXX check invalid file!
	ui->fileName->setText(file);
	QPixmap thumbnail(file);
	ui->thumbnail->setPixmap(thumbnail);
	ui->dimension->setText(QString::number(thumbnail.width()) + "x" + QString::number(thumbnail.height()));

	QStringList data;

	exiv2 = new QExiv2();
	if (exiv2->load(file)) {
		data = exiv2->xmpTagStringBag("Xmp.dc.subject", true);
		ui->ratingSpinBox->setValue(exiv2->xmpTagString("Xmp.xmp.Rating", true).toDouble());

		QString dsc = exiv2->getXmpTagStringLangAlt("Xmp.dc.description", QString(), false);
		ui->description->setPlainText(dsc);
	}

	m_model = new QStringListModel(this);
	m_model->setStringList(data);

	m_filter = new QSortFilterProxyModel(this);
	m_filter->setSourceModel(m_model);

	connect(ui->lineEdit, SIGNAL(textChanged(QString)),
		m_filter, SLOT(setFilterFixedString(QString)));

	connect(ui->description, SIGNAL(textChanged()),
		this, SLOT(descriptionChanged()));

	m_filter->setFilterCaseSensitivity(Qt::CaseInsensitive);
	ui->listView->setModel(m_filter);
}

MainWindow::~MainWindow()
{
	delete exiv2;
	delete ui;
}

void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{
	ui->lineEdit->setText(index.data().toString());
}

void MainWindow::on_addToolButton_clicked()
{
	QString str = ui->lineEdit->text();
	if (str.isEmpty())
		return;

	str = str.simplified();

	m_model->insertRows(m_model->rowCount(), 1);
	if (m_model->setData(m_model->index(m_model->rowCount() - 1), str)) {
		m_xmpUpdate = true;
		ui->saveButton->setEnabled(m_xmpUpdate);
	}

	ui->lineEdit->clear();
}

void MainWindow::on_removeToolButton_clicked()
{
	QModelIndex idx = ui->listView->currentIndex();
	if (m_model->removeRow(idx.row())) {
		m_xmpUpdate = true;
		ui->saveButton->setEnabled(m_xmpUpdate);
	}
}

void MainWindow::on_saveButton_clicked()
{
	if (m_xmpUpdate) {
		//m_model->sort(0);
		QString rating = QString::number(ui->ratingSpinBox->value(), 'f', 1);
		if (!exiv2->setXmpTagString("Xmp.xmp.Rating", rating)) {
			QMessageBox::critical(this, tr("Error"), tr("Cannot set Xmp.xmp.Rating"), QMessageBox::Abort);
			return;
		}

                if (!exiv2->setXmpTagStringLangAlt("Xmp.dc.description", ui->description->toPlainText(), QString())) {
			QMessageBox::critical(this, tr("Error"), tr("Cannot set Xmp.dc.description"), QMessageBox::Abort);
			return;
		}

		if (!exiv2->setXmpTagStringBag("Xmp.dc.subject", m_model->stringList())) {
			QMessageBox::critical(this, tr("Error"), tr("Cannot set Xmp.dc.subject"), QMessageBox::Abort);
			return;
		}

		exiv2->save();
	}
}

void MainWindow::on_ratingSpinBox_valueChanged(double)
{
	m_xmpUpdate = true;
	ui->saveButton->setEnabled(m_xmpUpdate);
}

void MainWindow::descriptionChanged()
{
	m_xmpUpdate = true;
	ui->saveButton->setEnabled(m_xmpUpdate);
}
