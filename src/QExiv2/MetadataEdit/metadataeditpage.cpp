#include "metadataeditpage.h"
#include "ui_metadataeditpage.h"

#include <QFileInfo>
#include <QDateTime>

#include <QDebug>

#include "QExiv2.h"
#include "md_keywordsdialog.h"
#include "md_datetimedialog.h"
#include "md_locationdialog.h"

#include "metadatalocation.h"
#include "geocoding.h"


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

    QGeoCoordinate gc;
    m_exiv2->geoCoordinate(&gc);

    if (gc.isValid()) {
        ui->xmpCoordinates->setText(gc.toString());
        //ui->exifCoordinates->setText(QString::number(gc.longitude()));
    }

    ui->ratingSpinBox->setValue(m_exiv2->xmpTagString("Xmp.xmp.Rating", true).toDouble());
    ui->Xmp_dc_description->setPlainText(m_exiv2->xmpTagStringLangAlt("Xmp.dc.description", QString(), false));
    ui->Xmp_dc_title->setText(m_exiv2->xmpTagStringLangAlt("Xmp.dc.title", QString(), false));
    ui->Xmp_iptcExt_Event->setText(m_exiv2->xmpTagStringLangAlt("Xmp.iptcExt.Event", QString(), false));

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
    QStringList files;
    files << m_fileName;
    LocationDialog dlg(files);
    if (QDialog::Accepted == dlg.exec()) {
        // Reload Data
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

void MetadataEditPage::on_ratingSpinBox_valueChanged(double)
{
    m_updateRating = true;
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
        e->setXmpTagStringLangAlt("Xmp.iptcExt.Event", ui->Xmp_iptcExt_Event->text(), QString());
    }

    if (ui->Xmp_dc_title->text().isEmpty()) {
        e->removeXmpTag("Xmp.dc.title");
    } else {
        e->setXmpTagStringLangAlt("Xmp.dc.title", ui->Xmp_dc_title->text(), QString());
    }

    if (ui->Xmp_dc_description->toPlainText().isEmpty()) {
        e->removeXmpTag("Xmp.dc.description");
    } else {
        e->setXmpTagStringLangAlt("Xmp.dc.description", ui->Xmp_dc_description->toPlainText(), QString());
    }

    if (m_updateRating) {
        e->setXmpTagString("Xmp.xmp.Rating", QString::number(ui->ratingSpinBox->value(), 'f', 1));
        m_updateRating = false;
    }

    if (e->save()) {
        delete e;
        return true;
    }

    qDebug() << __PRETTY_FUNCTION__ << "Error set Xmp Data on:" << m_fileName;
    delete e;
    return false;
}

void MetadataEditPage::on_tabWidget_currentChanged(int index)
{
    Q_UNUSED(index)
}


void MetadataEditPage::on_reverseGeocodingButton_clicked()
{

}
