#include "metadataeditpage.h"
#include "ui_metadataeditpage.h"

#include <QFileInfo>
#include <QDateTime>

#include <QDebug>

#include "QExiv2.h"
#include "md_keywordsdialog.h"
#include "md_datetimedialog.h"
#include "md_locationdialog.h"
#include "md_socialdialog.h"

MetadataEditPage::MetadataEditPage(QWidget *parent) :
	QWidget(parent),
    ui(new Ui::MetadataEditPage),
    m_exiv2(Q_NULLPTR)
{
	ui->setupUi(this);

    ui->reverseGeolocationText->clear();

    connect(&m_geoCoding, &GeoCoding::reverseGeocodeFinished,
            this, &MetadataEditPage::onReverseGeocodeFinished);
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
        m_exiv2 = Q_NULLPTR;
    }

    m_exiv2 = new QExiv2();
    if (!m_exiv2->load(m_fileName)) {
        delete m_exiv2;
        m_exiv2 = Q_NULLPTR;
        // Disable all widgets!!!
        return;
    }

    getDateTime();
    getLocations();

    m_exifGps = m_exiv2->exifGeoCoordinate();
    if (m_exifGps.isValid()) {
        ui->exifCoordinates->setText(m_exifGps.toString());
    } else {
        ui->exifCoordinates->clear();
    }
    m_xmpGps = m_exiv2->xmpGeoCoordinate();
    if (m_xmpGps.isValid()) {
        ui->xmpCoordinates->setText(m_xmpGps.toString());
    } else {
        ui->xmpCoordinates->clear();
    }

    ui->ratingSpinBox->setValue(m_exiv2->xmpTagString("Xmp.xmp.Rating", true).toDouble());
    ui->Xmp_dc_description->setPlainText(m_exiv2->xmpLangAlt("Xmp.dc.description", QString(), false));
    ui->Xmp_dc_title->setText(m_exiv2->xmpLangAlt("Xmp.dc.title", QString(), false));
    ui->Xmp_iptcExt_Event->setText(m_exiv2->xmpLangAlt("Xmp.iptcExt.Event", QString(), false));

    // Image Comment
    if (m_exiv2->hasComment()) {
        ui->imageComment->setPlainText(m_exiv2->imgComment());
    }
    if (!m_exiv2->isImgCommentWritable()) {
        ui->imageComment->setReadOnly(true);
        ui->imageCommentLabel->setText("Comment (ReadOnly)");
    }

    ui->keywordssTextEdit->clear();
    QStringList tags = m_exiv2->xmpTagStringBag("Xmp.dc.subject", true);
    for (int i = 0; i < tags.count(); i++) {
        ui->keywordssTextEdit->appendPlainText(tags.at(i));
    }

    delete m_exiv2;
    m_exiv2 = Q_NULLPTR;
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
    exif_dt = m_exiv2->datetime(QExiv2::ExifOriginal);
    iptc_dt = m_exiv2->datetime(QExiv2::IptcOriginal);
    xmp_dt  = m_exiv2->datetime(QExiv2::XmpOriginal);

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
    exif_dt = m_exiv2->datetime(QExiv2::ExifDigitized);
    iptc_dt = m_exiv2->datetime(QExiv2::IptcDigitized);
    xmp_dt  = m_exiv2->datetime(QExiv2::XmpDigitized);

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
    exif_dt = m_exiv2->datetime(QExiv2::ExifModified);
    xmp_dt  = m_exiv2->datetime(QExiv2::XmpModified);

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
    LocationDialog dlg(QStringList(m_fileName), this);
    if (QDialog::Accepted == dlg.exec()) {
        // Reload Data
        // getLocations();
        setFile(m_fileName);
    }
}

void MetadataEditPage::on_datetimeEditButton_clicked()
{
    MetadataDateTimeDialog dlg(QStringList(m_fileName), this);
    if (QDialog::Accepted == dlg.exec()) {
        // Reload Data
        setFile(m_fileName);
    }
}

void MetadataEditPage::on_keywordsEditButton_clicked()
{
    KeywordsDialog dlg(QStringList(m_fileName), this);
    if (QDialog::Accepted == dlg.exec()) {
        // Reload Data
        setFile(m_fileName);
    }
}

bool MetadataEditPage::saveMetadata()
{
    QExiv2 *e = new QExiv2();
    if (!e->load(m_fileName)) {
        delete e;
        return false;
    }

    e->setImgComment(ui->imageComment->toPlainText().toLatin1());

    if (ui->Xmp_iptcExt_Event->text().isEmpty()) {
        e->removeXmpTag("Xmp.iptcExt.Event");
    } else {
        e->setXmpLangAlt("Xmp.iptcExt.Event", ui->Xmp_iptcExt_Event->text());
    }

    if (ui->Xmp_dc_title->text().isEmpty()) {
        e->removeXmpTag("Xmp.dc.title");
    } else {
        e->setXmpLangAlt("Xmp.dc.title", ui->Xmp_dc_title->text());
    }

    if (ui->Xmp_dc_description->toPlainText().isEmpty()) {
        e->removeXmpTag("Xmp.dc.description");
    } else {
        e->setXmpLangAlt("Xmp.dc.description", ui->Xmp_dc_description->toPlainText());
    }

    // Spinbox range is defined on .ui file.
    e->setXmpTagString("Xmp.xmp.Rating", QString::number(ui->ratingSpinBox->value(), 'f', 1));

    if (e->save()) {
        delete e;
        return true;
    }

    qDebug() << __PRETTY_FUNCTION__ << "Error set Xmp Data on:" << m_fileName;
    delete e;
    return false;
}

void MetadataEditPage::on_reverseGeolocationButton_clicked()
{
    QGeoCoordinate gc;
    if (ui->useXmpGPS->isChecked()) {
        gc =m_xmpGps;
    } else {
        gc = m_exifGps;
    }

    if (gc.isValid()) {
        m_geoCoding.reverseGeoCode(gc);
    }
}

void MetadataEditPage::onReverseGeocodeFinished()
{
    //qDebug() << m_geoCoding.location().address().text();
    ui->reverseGeolocationText->setText(m_geoCoding.location().address().text());
}

void MetadataEditPage::on_socialMetadataEditButton_clicked()
{
    SocialMetadataDialog dlg(QStringList(m_fileName), this);
    dlg.exec();
}
