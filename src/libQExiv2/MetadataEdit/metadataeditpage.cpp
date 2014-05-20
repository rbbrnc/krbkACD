#include "metadataeditpage.h"
#include "ui_metadataeditpage.h"

#include <QFileInfo>
#include <QDateTime>

#include <QDebug>

#include "QExiv2.h"
#include "LocationDialog.h"
#include "datetimedialog.h"
#include "metadatalocation.h"

MetadataEditPage::MetadataEditPage(QWidget *parent) :
	QWidget(parent),
    ui(new Ui::MetadataEditPage),
    m_exiv2(0)
{
	ui->setupUi(this);
}

MetadataEditPage::~MetadataEditPage()
{
	delete ui;
}

void MetadataEditPage::setFile(const QString &file)
{
    m_fileName = file;

    if (m_exiv2) {
        delete m_exiv2;
        m_exiv2 = 0;
    }

    m_exiv2 = new QExiv2();
    if (!m_exiv2->load(m_fileName)) {
        delete m_exiv2;
        m_exiv2 = 0;
        // Disable all widgets!!!
        return;
    }

    getDateTime();
    getLocations();

    ui->ratingSpinBox->setValue(m_exiv2->xmpTagString("Xmp.xmp.Rating", true).toDouble());

    delete m_exiv2;
    m_exiv2 = 0;
}

void MetadataEditPage::getLocations()
{
    MetadataLocation lc;
    MetadataLocation ls;

    m_exiv2->locationCreated(lc, 1);
    m_exiv2->locationShown(ls, 1);

    ui->locationCreated->setText(lc.toString());
    ui->locationShown->setText(ls.toString());
}

void MetadataEditPage::getDateTime()
{
    QDateTime exif_dt;
    QDateTime iptc_dt;
    QDateTime xmp_dt;

    // Creation date of the intellectual content
    // (e.g. when a photo was taken)
    exif_dt = m_exiv2->exifDateTimeOriginal();
    iptc_dt = m_exiv2->iptcDateTimeCreated();
    xmp_dt  = m_exiv2->xmpDateTimeCreated();

    if (exif_dt.isValid()) {
        ui->datetimeOriginal->setText(exif_dt.toString(Qt::ISODate));
    } else if (xmp_dt.isValid()) {
        ui->datetimeOriginal->setText(xmp_dt.toString(Qt::ISODate));
    } else if (iptc_dt.isValid()) {
        ui->datetimeOriginal->setText(iptc_dt.toString(Qt::ISODate));
    } else {
        ui->datetimeOriginal->clear();
    }

/*
    qDebug() << "[EXIF]" << exif_dt.toString(Qt::ISODate)
             << "[IPTC]" << iptc_dt.toString(Qt::ISODate)
             << "[XMP ]" << xmp_dt.toString(Qt::ISODate);
*/

    // Creation date of the digital representation
    // (e.g. when an image was digitized)
    exif_dt = m_exiv2->exifDateTimeDigitized();
    iptc_dt = m_exiv2->iptcDateTimeDigitized();
    xmp_dt  = m_exiv2->xmpDateTimeDigitized();

    if (exif_dt.isValid()) {
        ui->datetimeDigitized->setText(exif_dt.toString(Qt::ISODate));
    } else if (xmp_dt.isValid()) {
        ui->datetimeDigitized->setText(xmp_dt.toString(Qt::ISODate));
    } else if (iptc_dt.isValid()) {
        ui->datetimeDigitized->setText(iptc_dt.toString(Qt::ISODate));
    } else {
        ui->datetimeDigitized->clear();
    }

/*
    qDebug() << "[EXIF]" << exif_dt.toString(Qt::ISODate)
             << "[IPTC]" << iptc_dt.toString(Qt::ISODate)
             << "[XMP ]" << xmp_dt.toString(Qt::ISODate);
*/

    // Modification date of the digital image file
    // (e.g. when a file was modified by the user)
    exif_dt = m_exiv2->exifDateTime();
    xmp_dt = m_exiv2->xmpDateTimeModified();

    if (exif_dt.isValid()) {
        ui->datetimeModified->setText(exif_dt.toString(Qt::ISODate));
    } else if (xmp_dt.isValid()) {
        ui->datetimeModified->setText(xmp_dt.toString(Qt::ISODate));
    } else {
        ui->datetimeModified->clear();
    }
/*
    qDebug() << "[EXIF]" << exif_dt.toString(Qt::ISODate)
             << "[IPTC]" << iptc_dt.toString(Qt::ISODate)
             << "[XMP ]" << xmp_dt.toString(Qt::ISODate);
*/
}

void MetadataEditPage::on_locationEditButton_clicked()
{
    QStringList files;
    files << m_fileName;
    LocationDialog dlg(files);
    dlg.exec();
}

void MetadataEditPage::on_datetimeEditButton_clicked()
{
    MetadataDateTimeDialog dlg(m_fileName);
    dlg.exec();
}
