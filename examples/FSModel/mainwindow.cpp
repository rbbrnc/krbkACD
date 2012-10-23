#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "fsManager.h"
#include <QDebug>
#include <QMessageBox>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	m_fs = FSManager::instance();
//	m_fs->currentIndex();

	ui->listView->setModel(m_fs->model());
	ui->listView->setRootIndex(m_fs->currentIndex());
	ui->listView->setSelectionMode(QAbstractItemView::SingleSelection);

	m_selectionModel = ui->listView->selectionModel();

	connect(m_selectionModel,
		SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
		this,
		SLOT(fileSelect(const QModelIndex &, const QModelIndex &)));

	connect(ui->listView, SIGNAL(activated(QModelIndex)),
		this, SLOT(handleItemActivation(QModelIndex)));

	connect(ui->hiddenButton, SIGNAL(toggled(bool)), m_fs, SLOT(showHiddenFiles(bool)));
	connect(ui->iconButton, SIGNAL(toggled(bool)), this, SLOT(iconMode(bool)));
	connect(ui->mkDirButton, SIGNAL(clicked()), this, SLOT(mkDir()));
	connect(ui->removeButton, SIGNAL(clicked()), this, SLOT(remove()));
	connect(ui->renameButton, SIGNAL(clicked()), this, SLOT(rename()));
}

MainWindow::~MainWindow()
{
	delete ui;
}

/*  SLOT [private] */
void MainWindow::fileSelect(const QModelIndex &current, const QModelIndex &/*previous*/)
{
	ui->label->setText(m_fs->model()->fileName(current));
}

/*  SLOT [public]
 *
 *  Changes current directory
 *
 * @param path directory to change to
 */
void MainWindow::changePath(const QString &path)
{
        m_currentDir.cd(path);
        QString newPath = m_currentDir.absolutePath();

	m_fs->changePath(newPath);
        ui->listView->clearSelection();

        ui->listView->setRootIndex(m_fs->currentIndex());

        //emit pathChanged(newPath);
        ui->listView->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->listView->setCurrentIndex(m_fs->firstRow());
}
/*  SLOT [private] */
void MainWindow::handleItemActivation(QModelIndex index)
{
	if (ui->listView->selectionMode() == QAbstractItemView::SingleSelection) {
		QFileInfo file = m_fs->model()->fileInfo(index);
		if (file.isDir()) {
			changePath(file.absoluteFilePath());
		}
	}
	ui->listView->setSelectionMode(QAbstractItemView::SingleSelection);
}

void MainWindow::on_prevButton_clicked()
{
	//QModelIndex mi = m->previousRow(ui->listView->currentIndex());
	QModelIndex mi = m_fs->previousRow(ui->listView->currentIndex().row());
	if (mi.isValid()) {
		ui->listView->setCurrentIndex(mi);
		ui->label->setText(m_fs->model()->fileName(mi));
	}
}

void MainWindow::on_nextButton_clicked()
{
	//QModelIndex mi = m->nextRow(ui->listView->currentIndex());
	QModelIndex mi = m_fs->nextRow(ui->listView->currentIndex().row());
	if (mi.isValid()) {
		ui->listView->setCurrentIndex(mi);
		ui->label->setText(m_fs->model()->fileName(mi));
	}
}

void MainWindow::on_pushButton_clicked()
{
	QModelIndex index = m_fs->firstRow();
	do {
		if (index.isValid())
			qDebug() << m_fs->model()->fileName(index);
		else
			break;

		index = m_fs->nextRow(index);
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

// [SLOT private]
void MainWindow::mkDir()
{
	QString dirName = QInputDialog::getText(this,
				QObject::tr("Create a new directory"),
				QObject::tr("Enter directory name: "),
				QLineEdit::Normal);
	// Remove whitespaces
	dirName = dirName.simplified();

	if (dirName.isEmpty()) {
		return;
	}

	QModelIndex index = m_fs->model()->mkdir(m_fs->currentIndex(), dirName);
	if (!index.isValid()) {
		QMessageBox::critical(this,
			QObject::tr("Error"),
			QObject::tr("Creating dir '%1' failed").arg(dirName),
			QMessageBox::Abort);
	}
}

// [SLOT private]
void MainWindow::remove()
{
	QModelIndex index = ui->listView->currentIndex();
	QString msg;

	if (m_fs->model()->isDir(index)) {
		msg = tr("Delete Directory %1?").arg(m_fs->model()->fileName(index));
	} else {
		msg = tr("Delete File %1?").arg(m_fs->model()->fileName(index));
	}

	if (QMessageBox::No == QMessageBox::warning(this, "Delete", msg, QMessageBox::Yes, QMessageBox::No)) {
		return;
	}

	if (m_fs->model()->remove(index)) {
		return;
	}

	QMessageBox::critical(this, QObject::tr("Error"),
		tr("Cannot Remove '%1'").arg(m_fs->model()->filePath(index)),
		QMessageBox::Abort);
}

// [SLOT private]
void MainWindow::rename()
{
	qDebug() << __PRETTY_FUNCTION__;
	QModelIndexList m_selection = m_selectionModel->selectedIndexes();
	while (!m_selection.isEmpty()) {
		qDebug() << m_fs->model()->fileName(m_selection.first());
		m_selection.removeFirst();
	}

}
