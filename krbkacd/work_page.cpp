#include "work_page.h"
#include "ui_work_page.h"

#include <QMessageBox>
#include <QDebug>

// TODO: Button for stopping the scan
//
WorkPage::WorkPage(QWidget *parent) :
	QWidget(parent), ui(new Ui::WorkPage)
{
	ui->setupUi(this);

	m_dirModel.setRootPath("");
	m_dirModel.setFilter(QDir::Dirs | QDir::NoDotAndDotDot); // Only Directories

	ui->dirTree->setModel(&m_dirModel);

	// Do it after the model has been initialized
	ui->dirTree->header()->hide();
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
	m_dupMap.clear();
	for (int i = 0; i < ui->dirList->count(); i++) {
		QString path = ui->dirList->item(i)->text();
		scanDir(path, ui->recursiveCheck->isChecked());
	}

	if (m_dupMap.isEmpty()) {
		qDebug() << "No file found!";
	} else {
		// (Debug) Map
		qDebug() << "-------RESULTS-----------";
		QMap<QString, QStringList>::const_iterator i = m_dupMap.constBegin();
		 while (i != m_dupMap.constEnd()) {
			if (i.value().size() > 1) {
				qDebug() << i.key() << ": " << i.value();
			}
			++i;
		 }
		qDebug() << "-------------------------";
	}
}

void WorkPage::scanDir(const QString path, bool recursive)
{
	QDir dir;
	dir.setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
	dir.setCurrent(path);

	QFileInfoList list = dir.entryInfoList();

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
			//qDebug() << "FILE:" << qPrintable(fi.absoluteFilePath());

			// Check mime filter
			if (ui->mimeFilterImages->isChecked()) {
				if (!fd.isImage()) {
					continue;
				}
			}

			// TODO: For MD5, Image and Byte-to-Byte compare only files with the same format
			switch (ui->compareType->currentIndex()) {
			case 0:
				// Compare Method: "File MD5"
				compareFileMd5(fd, m_dupMap);
				break;
			case 1:
				// Compare Method: "Image, Only skip Metadata"
				compareImage(fd, m_dupMap);
				break;
			case 2:
				// Compare Method: "Metadata Only Skip Image Data
				compareMetadata(fd, m_dupMap);
				break;
			case 3:
				// Compare Method: "Byte-to-Byte"
				compareByteToByte(fd, m_dupMap);
				break;
			default:
				qDebug() << "Something wrong!";
				break;
			}
		}
	}
}

// Compare Method: "File MD5"
void WorkPage::compareFileMd5(FileData &fdata, QMap<QString, QStringList> &map)
{
	QString md5;
	md5 = fdata.md5().toHex();

	QStringList sl;

	if (map.contains(md5)) {
		// Duplicate Found get file list for this <key>
		sl = map[md5];
	}

	sl << fdata.filePath();	// Update file list
	map[md5] = sl;		// Add to map
}

// Compare Method: "Metadata Only, skip Image Data
void WorkPage::compareMetadata(FileData &fdata, QMap<QString, QStringList> &map)
{
	Q_UNUSED(fdata)
	Q_UNUSED(map)

	qDebug() << "WorkPage::CompareMetadata";
}

// Compare Method: "Byte-to-Byte"
void WorkPage::compareByteToByte(FileData &fdata, QMap<QString, QStringList> &map)
{
	Q_UNUSED(fdata)
	Q_UNUSED(map)

	qDebug() << "WorkPage::CompareByteToByte";
}

// Compare Method: "Image Only skip Metadata"
void WorkPage::compareImage(FileData &fdata, QMap<QString, QStringList> &map)
{
	Q_UNUSED(fdata)
	Q_UNUSED(map)

	qDebug() << "WorkPage::CompareImage";
}

