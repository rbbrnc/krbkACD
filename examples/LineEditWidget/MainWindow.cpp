#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QStringListModel>
#include <QSortFilterProxyModel>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	QStringList data;
	data << "Alex" << "Bob" << "Celeste" << "David"
         << "Emil" << "Frederik" << "Gerd" << "Harald" << "Ivan";

	m_model = new QStringListModel(this);
	m_model->setStringList(data);

	m_filter = new QSortFilterProxyModel(this);
	m_filter->setSourceModel(m_model);

	connect(ui->lineEdit, SIGNAL(textChanged(QString)),
		m_filter, SLOT(setFilterFixedString(QString)));

	m_filter->setFilterCaseSensitivity(Qt::CaseInsensitive);
	ui->listView->setModel(m_filter);
}

MainWindow::~MainWindow()
{
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
	m_model->setData(m_model->index(m_model->rowCount() - 1), str);

#if 0
	m_model->sort(0);
#endif

	ui->lineEdit->clear();
}

void MainWindow::on_removeToolButton_clicked()
{
	QModelIndex idx = ui->listView->currentIndex();
	m_model->removeRow(idx.row());
}

void MainWindow::on_listView_clicked(const QModelIndex &index)
{
	qDebug() << __func__ << index.data();
}
