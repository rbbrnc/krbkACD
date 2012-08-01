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

	ui->tresholdLayout->hide();

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
		//qDebug() << __PRETTY_FUNCTION__ << "Insert item:" << m_currentDirSelection;
	}
}

// Remove entry from the selected directory list
void WorkPage::on_removeDir_clicked()
{
	int row = ui->dirList->currentRow();
	delete ui->dirList->takeItem(row);
	//qDebug() << __PRETTY_FUNCTION__ << "Remove item on row:" << row;
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

void WorkPage::debugDupMap() const
{
	qDebug() << "-------" << __PRETTY_FUNCTION__ << "-----------";
	if (m_dupMap.isEmpty()) {
		qDebug() << "No file found!";
	} else {
		// (Debug) Map
		QMap<QString, QStringList>::const_iterator i = m_dupMap.constBegin();
		 while (i != m_dupMap.constEnd()) {
			if (i.value().size() > 1) {
				qDebug() << i.key() << ": " << i.value();
			}
			++i;
		 }
	}
	qDebug() << "-------------------------";
}

void WorkPage::debugHistogramMap() const
{
	qDebug() << "-------" << __PRETTY_FUNCTION__ << "-----------";
	if (m_histogramMap.isEmpty()) {
		qDebug() << "No histogram found!";
	} else {
		// (Debug) Map
		QMap<QString, ColorHistogram>::const_iterator i = m_histogramMap.constBegin();
		 while (i != m_histogramMap.constEnd()) {
			qDebug() << i.key(); // << ": " << i.value();
			++i;
		 }
	}
	qDebug() << "-------------------------";
}

void WorkPage::debugFileList() const
{
	qDebug() << "-------" << __PRETTY_FUNCTION__ << "-----------";
	if (m_fileList.isEmpty()) {
		qDebug() << "No file found!";
	} else {
		for (int i = 0; i < m_fileList.size(); i++) {
			qDebug() << m_fileList.at(i).filePath();
		 }
	}
	qDebug() << "-------------------------";
}

void WorkPage::on_workButton_clicked()
{
	if (!m_fileList.isEmpty()) {
		m_fileList.clear();
	}

	for (int i = 0; i < ui->dirList->count(); i++) {
		QString path = ui->dirList->item(i)->text();
		fillFileList(path, ui->recursiveCheck->isChecked());
	}
	//debugFileList();

	if (m_fileList.isEmpty()) {
		QMessageBox::information(this, tr("Find Duplicate"), "No Image in Directory List");
		return;
	}

	m_dupMap.clear();
	m_histogramMap.clear();

	// TODO: For MD5, Image and Byte-to-Byte compare only files with the same format
	switch (ui->compareType->currentIndex()) {
	case 0:
		// Compare Method: "File MD5"
		compareFileMd5();
		break;
	case 1:
		// Compare Method: "Color Histogram"
		compareHistogram();
		break;
	case 2:
		// Compare Method: "Image, Only skip Metadata"
		compareImage();
		break;
	case 3:
		// Compare Method: "Metadata Only Skip Image Data
		compareMetadata();
		break;
	case 4:
		// Compare Method: "Byte-to-Byte"
		compareByteToByte();
		break;
	default:
		qDebug() << __PRETTY_FUNCTION__ << "Something wrong!";
		break;
	}
}

void WorkPage::fillFileList(const QString path, bool recursive)
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
					fillFileList(fi.absoluteFilePath(), recursive);
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
			m_fileList << fd;
		}
	}
}

void WorkPage::on_compareType_currentIndexChanged(int index)
{
	if (index == 1) {
		ui->tresholdValue->setText(QString::number(ui->tresholdHistogramDiffSlider->value()));
		ui->tresholdLayout->show();
	} else {
		ui->tresholdLayout->hide();
	}
}

// Compare Method: "File MD5"
void WorkPage::compareFileMd5()
{
	for (int i = 0; i < m_fileList.size(); i++) {
		FileData fd = m_fileList.at(i);
		QString md5;
		md5 = fd.md5().toHex();

		QStringList sl;

		if (m_dupMap.contains(md5)) {
			// Duplicate Found get file list for this <key>
			sl = m_dupMap[md5];
		}

		sl << fd.filePath();	// Update file list
		m_dupMap[md5] = sl;	// Add to map
	}
	debugDupMap();
}

// Compare Method: "Color Histogtram"
void WorkPage::compareHistogram()
{
	for (int i = 0; i < m_fileList.size(); i++) {
		FileData fd = m_fileList.at(i);

		QImage img = fd.image();

		if (img.isNull()) {
			continue;
		}

		ColorHistogram h;
		h.calcNormalized(&img);

		QString fp = fd.filePath();

		m_histogramMap[fp] = h;		// Add to map
	}
	ColorHistogram h1;
	ColorHistogram h2;

	qDebug() << __PRETTY_FUNCTION__ << "Treshold:" << ui->tresholdHistogramDiffSlider->value();
	QMap<QString, ColorHistogram>::const_iterator i = m_histogramMap.constBegin();
	QMap<QString, ColorHistogram>::const_iterator j = m_histogramMap.constBegin();
	while (i != m_histogramMap.constEnd()) {
		j = i + 1;
		h1 = i.value();
		while (j != m_histogramMap.constEnd()) {
			h2 = j.value();
			double lhn = h1.compareChiSquareNormalized(h2);
			if (ui->tresholdHistogramDiffSlider->value() < ((1.0 - lhn)*100.0))
				qDebug() << i.key()
					 << j.key()
					 << (1.0 - lhn)*100.0 << "%";
			++j;
		}
		i++;
	}
//	debugHistogramMap();
}

// Compare Method: "Metadata Only, skip Image Data
void WorkPage::compareMetadata()
{
	qDebug() << __PRETTY_FUNCTION__;
}

// Compare Method: "Byte-to-Byte"
void WorkPage::compareByteToByte()
{
	qDebug() << __PRETTY_FUNCTION__;
}

// Compare Method: "Image Only skip Metadata"
void WorkPage::compareImage()
{
	for (int i = 0; i < m_fileList.size(); i++) {
		FileData fd = m_fileList.at(i);
		QString md5;
		md5 = fd.imageMd5().toHex();

//		qDebug() << __PRETTY_FUNCTION__ << fd.filePath() << "MD5" << md5;

		QStringList sl;

		if (m_dupMap.contains(md5)) {
			// Duplicate Found get file list for this <key>
			sl = m_dupMap[md5];
		}

		sl << fd.filePath();	// Update file list
		m_dupMap[md5] = sl;	// Add to map
	}
	debugDupMap();
}

void WorkPage::on_tresholdHistogramDiffSlider_valueChanged(int value)
{
	ui->tresholdValue->setText(QString::number(value));
}

void WorkPage::on_resultsButton_clicked()
{
	emit changePage(4);
}

