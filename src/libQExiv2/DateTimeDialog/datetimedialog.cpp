#include "datetimedialog.h"
#include "ui_datetimedialog.h"

#include <QDebug>
#include <QMessageBox>

#include "QExiv2.h"

MetadataDateTimeDialog::MetadataDateTimeDialog(const QString &file, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MetadataDateTimeDialog)
{
    ui->setupUi(this);

    m_fileList << file;
    loadData(file);
}

MetadataDateTimeDialog::MetadataDateTimeDialog(const QStringList &files, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::MetadataDateTimeDialog),
	m_fileList(files)
{
	ui->setupUi(this);

	if (m_fileList.count() == 0) {
		//"Empty selection!"
	} else if (m_fileList.count() == 1) {
		loadData(m_fileList.at(0));
	} else {
		// Multiple Selection
	}
}

MetadataDateTimeDialog::~MetadataDateTimeDialog()
{
	delete ui;
}

bool MetadataDateTimeDialog::loadData(const QString &file)
{
	QExiv2 *e = new QExiv2();
	if (!e->load(file)) {
		delete e;
		return false;
	}

#if 0
    // Creation date of the intellectual content
    // (e.g. when a photo was taken)
    ui->exifDateTimeOriginal->setText(e->exifDateTimeOriginal().toString(Qt::ISODate));
    ui->iptcDateCreated->setText(e->iptcDateTimeCreated().toString(Qt::ISODate));
    ui->xmpPhotoshopDateCreated->setText(e->xmpDateTimeCreated().toString(Qt::ISODate));

    ui->exifDateTimeDigitized->setText(e->exifDateTimeDigitized().toString(Qt::ISODate));
    ui->iptcDigitalCreationDate->setText(e->iptcDateTimeDigitized().toString(Qt::ISODate));
    ui->xmpCreateDate->setText(e->xmpDateTimeDigitized().toString(Qt::ISODate));

    ui->exifDateTime->setText(e->exifDateTime().toString(Qt::ISODate));
    ui->xmpModifyDate->setText(e->xmpDateTimeModified().toString(Qt::ISODate));

    delete e;
    return true;
#endif

	// Creation date of the intellectual content (e.g. when a photo was taken)
	ui->exifDateTimeOriginal->setText(e->exifTagString("Exif.Photo.DateTimeOriginal"));
//	ui->exifDateTimeOriginal->setText(e->exifTagString("Exif.Image.DateTimeOriginal"));

	QString iptcDateTime = e->iptcTagString("Iptc.Application2.DateCreated") + " " +
			       e->iptcTagString("Iptc.Application2.TimeCreated");
	ui->iptcDateCreated->setText(iptcDateTime);

    ui->xmpPhotoshopDateCreated->setText(e->xmpDateTimeCreated().toString(Qt::ISODate));

	// Creation date of the digital representation (e.g. when an image was digitized)
	ui->exifDateTimeDigitized->setText(e->exifTagString("Exif.Photo.DateTimeDigitized"));

	iptcDateTime = e->iptcTagString("Iptc.Application2.DigitizationDate")+ " " +
		       e->iptcTagString("Iptc.Application2.DigitizationTime");
	ui->iptcDigitalCreationDate->setText(iptcDateTime);

	ui->xmpCreateDate->setText(e->xmpTagString("Xmp.xmp.CreateDate"));

	// Modification date of the digital image file (e.g. when a file was modified by the user)
	ui->exifDateTime->setText(e->exifTagString("Exif.Image.DateTime"));
	ui->xmpModifyDate->setText(e->xmpTagString("Xmp.xmp.ModifyDate"));

	delete e;
	return true;
}

bool MetadataDateTimeDialog::saveData(const QString &file)
{
	bool rc = true;
	QDateTime dateTime = QDateTime::fromString(ui->xmpPhotoshopDateCreated->text(), Qt::ISODate);
	if (dateTime.isValid()) {
		if (dateTime != m_dateTime) {
			QExiv2 *e = new QExiv2();
			if (e->load(file)) {
				e->setXmpTagString("Xmp.photoshop.DateCreated", dateTime.toString(Qt::ISODate));
				rc = e->save();
			}
			delete e;
		}
	} else {
		if (dateTime != m_dateTime) {
			QMessageBox::warning(this, "ERROR", QString("Invalid DateTime: %1").arg(dateTime.toString(Qt::ISODate)));
			return false;
		}
	}

	return rc;
}

void MetadataDateTimeDialog::accept()
{
	for (int i = 0; i < m_fileList.count(); i++) {
		if (!saveData(m_fileList.at(i))) {
			return;
		}
	}
	QDialog::accept();
}

void MetadataDateTimeDialog::reject()
{
	emit rejected();
	QDialog::reject();
}

