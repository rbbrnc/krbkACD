#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "fsManager.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	FSManager *mgr = FSManager::instance();

	m_model = mgr->model();

	mgr->currentIndex();

	ui->listView->setModel(m_model);
	ui->listView->setRootIndex(mgr->currentIndex());
	ui->listView->setSelectionMode(QAbstractItemView::SingleSelection);

	m_selectionModel = ui->listView->selectionModel();

	connect(m_selectionModel,
		SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
		this,
		SLOT(fileSelect(const QModelIndex &, const QModelIndex &)));

	connect(ui->listView, SIGNAL(activated(QModelIndex)),
		this, SLOT(handleItemActivation(QModelIndex)));

	connect(ui->hiddenButton, SIGNAL(toggled(bool)),
		mgr /*FSManager::instance()*/, SLOT(showHiddenFiles(bool)));

	connect(ui->iconButton, SIGNAL(toggled(bool)), this, SLOT(iconMode(bool)));

}

MainWindow::~MainWindow()
{
	delete ui;
}

/*  SLOT [private] */
void MainWindow::fileSelect(const QModelIndex &current, const QModelIndex &/*previous*/)
{
	FSManager *mgr = FSManager::instance();
	ui->label->setText(mgr->model()->fileName(current));
}

/*  SLOT [public]
 *
 *  Changes current directory
 *
 * @param path directory to change to
 */
void MainWindow::changePath(const QString &path)
{
	FSManager *mgr = FSManager::instance();

        m_currentDir.cd(path);
        QString newPath = m_currentDir.absolutePath();

	mgr->changePath(newPath);
        ui->listView->clearSelection();

        ui->listView->setRootIndex(mgr->currentIndex());

        //emit pathChanged(newPath);
        ui->listView->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->listView->setCurrentIndex(mgr->firstRow());
}
/*  SLOT [private] */
void MainWindow::handleItemActivation(QModelIndex index)
{
	if (ui->listView->selectionMode() == QAbstractItemView::SingleSelection) {
		QFileInfo file = m_model->fileInfo(index);
		if (file.isDir()) {
			changePath(file.absoluteFilePath());
		}
	}
	ui->listView->setSelectionMode(QAbstractItemView::SingleSelection);
}

void MainWindow::on_prevButton_clicked()
{
	FSManager *mgr = FSManager::instance();
	//QModelIndex mi = m->previousRow(ui->listView->currentIndex());
	QModelIndex mi = mgr->previousRow(ui->listView->currentIndex().row());
	if (mi.isValid()) {
		ui->listView->setCurrentIndex(mi);
		ui->label->setText(m_model->fileName(mi));
	}
}

void MainWindow::on_nextButton_clicked()
{
	FSManager *mgr = FSManager::instance();
	//QModelIndex mi = m->nextRow(ui->listView->currentIndex());
	QModelIndex mi = mgr->nextRow(ui->listView->currentIndex().row());
	if (mi.isValid()) {
		ui->listView->setCurrentIndex(mi);
		ui->label->setText(m_model->fileName(mi));
	}
}

void MainWindow::on_pushButton_clicked()
{
	FSManager *mgr = FSManager::instance();
	QModelIndex index = mgr->firstRow();
	do {
		if (index.isValid())
			qDebug() << mgr->model()->fileName(index);
		else
			break;

		index = mgr->nextRow(index);
	} while (index.isValid());
}


// [SLOT public]
//
// Switches view mode
// @param iconmode true shows iconview, false shows listview
//
void MainWindow::iconMode(bool enable)
{
	if (enable) {
		ui->listView->setViewMode(QListView::IconMode);
                ui->listView->setWordWrap(true);
		ui->listView->setGridSize(QSize(80,80));
	} else {
		ui->listView->setViewMode(QListView::ListMode);
		ui->listView->setWordWrap(false);
		ui->listView->setGridSize(QSize());
	}
}



void MainWindow::keyPressEvent(QKeyEvent *event)
{
	if ((event->key() == Qt::Key_Control) || (event->key() == Qt::Key_Shift)) {
		ui->listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
	} else {
		QWidget::keyPressEvent(event);
	}
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
	if ((event->key() == Qt::Key_Control) || (event->key() == Qt::Key_Shift)) {
		ui->listView->setSelectionMode(QAbstractItemView::SingleSelection);
	} else {
		QWidget::keyPressEvent(event);
	}
}

