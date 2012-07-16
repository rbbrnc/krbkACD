#include "work_page.h"
#include "ui_work_page.h"

#include <QMessageBox>
#include <QDebug>

#include "file_data.h"

// TODO: Button for stopping the scan
//
//
WorkPage::WorkPage(QWidget *parent) :
	QWidget(parent), ui(new Ui::WorkPage)
{
	ui->setupUi(this);

	m_dirModel.setRootPath("");
	m_dirModel.setFilter(QDir::Dirs | QDir::NoDotAndDotDot); // Only Directories

	ui->dirTree->setModel(&m_dirModel);

	// Do it after the model has been initialized
	ui->dirTree->hideColumn(1);
	ui->dirTree->hideColumn(2);
	ui->dirTree->hideColumn(3);

	connect(ui->dirTree, SIGNAL(clicked(const QModelIndex &)),
		this, SLOT(dirSelectionChanged(const QModelIndex &)));
}

WorkPage::~WorkPage()
{
	delete ui;
}

// Selection changed on directory tree
void WorkPage::dirSelectionChanged(const QModelIndex &index)
{
	QFileInfo fi = m_dirModel.fileInfo(index);
	m_currentDirSelection = fi.absoluteFilePath();
}

// Insert entry into the selected directory list
void WorkPage::on_insertDir_clicked()
{
	QList<QListWidgetItem *> lst;
	lst = ui->dirList->findItems(m_currentDirSelection, Qt::MatchExactly);

	// Do not add duplicate item
	if (lst.isEmpty()) {
		ui->dirList->addItem(m_currentDirSelection);
		//qDebug() << __func__ << "Insert item:" << m_currentDirSelection;
	}
}

// Remove entry from the selected directory list
void WorkPage::on_removeDir_clicked()
{
	int row = ui->dirList->currentRow();
	delete ui->dirList->takeItem(row);
	//qDebug() << __func__ << "Remove item on row:" << row;
}

// Clear the selected directory list
void WorkPage::on_clearDir_clicked()
{
	if (ui->dirList->count()) {
		if (QMessageBox::Yes == QMessageBox::question(this, tr("Clear List"), "Clear Directory List?", QMessageBox::Yes, QMessageBox::No)) {
			ui->dirList->clear();
		}
	}
}

void WorkPage::on_workButton_clicked()
{
	//qDebug() << __func__;
	for (int i = 0; i < ui->dirList->count(); i++) {
		QString path = ui->dirList->item(i)->text();
		scanDir(path, ui->recursiveCheck->isChecked());
	}
}

void WorkPage::scanDir(const QString path, bool recursive)
{
	QDir dir;
	dir.setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
	dir.setCurrent(path);

	QFileInfoList list = dir.entryInfoList();
	QStringList workingList;

//	qDebug() << "DIR:" << qPrintable(QDir::current().path());

	for (int i = 0; i < list.size(); ++i) {
		QFileInfo fi = list.at(i);
		if (fi.isDir()) {
			if ((fi.fileName() != ".") && (fi.fileName() != "..")) {
				if (recursive) {
					QDir::setCurrent(fi.absoluteFilePath());
					scanDir(fi.absoluteFilePath(), recursive);
					QDir::setCurrent("..");
				}
			}
		} else {
			FileData fd(fi);
			if (ui->mimeFilterImages->isChecked()) {
				if (!fd.isImage()) {
					continue;
				} else {
					fd.print();
					workingList << qPrintable(fi.absoluteFilePath());
				}
			}
			//qDebug() << "FILE:" << qPrintable(fi.absoluteFilePath());
		}
	}

	qDebug() << "workingList:" << workingList;
}

