#include "md_datetimedialog.h"
#include "ui_md_datetimedialog.h"

#include <QDebug>
#include <QMessageBox>

#include "QExiv2.h"

MetadataDateTimeDialog::MetadataDateTimeDialog(const QStringList &files, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::MetadataDateTimeDialog),
    m_fileList(files),
    m_exiv2(0)
{
	ui->setupUi(this);

    if (m_fileList.isEmpty()) {
        // Empty selection! Disable all widgets"
        enableWidgets(false);
        return;
    }

    m_exiv2 = new QExiv2();
    if (m_fileList.count() == 1) {
        if (!m_exiv2->load(m_fileList.first())) {
            // Disable widgets
            enableWidgets(false);
        } else {
            ui->exifOriginal->setText(m_exiv2->datetime(QExiv2::ExifOriginal).toString(Qt::ISODate));
            ui->iptcOriginal->setText(m_exiv2->datetime(QExiv2::IptcOriginal).toString(Qt::ISODate));
            ui->xmpOriginal->setText(m_exiv2->datetime(QExiv2::XmpOriginal).toString(Qt::ISODate));

            ui->exifDigitized->setText(m_exiv2->datetime(QExiv2::ExifDigitized).toString(Qt::ISODate));
            ui->iptcDigitized->setText(m_exiv2->datetime(QExiv2::IptcDigitized).toString(Qt::ISODate));
            ui->xmpDigitized->setText(m_exiv2->datetime(QExiv2::XmpDigitized).toString(Qt::ISODate));

            ui->exifModified->setText(m_exiv2->datetime(QExiv2::ExifModified).toString(Qt::ISODate));
            ui->xmpModified->setText(m_exiv2->datetime(QExiv2::XmpModified).toString(Qt::ISODate));
        }
	} else {
        // Multiple Selection (Enable Only Date and disable Time Edit?)
	}
}

MetadataDateTimeDialog::~MetadataDateTimeDialog()
{
    if (m_exiv2) {
        delete m_exiv2;
    }
	delete ui;
}

void MetadataDateTimeDialog::enableWidgets(bool enable)
{
    ui->xmpOriginal->setEnabled(enable);
}

bool MetadataDateTimeDialog::saveData()
{
    QDateTime oldDatetime = m_exiv2->datetime(QExiv2::XmpOriginal);
    QDateTime newDatetime = QDateTime::fromString(ui->xmpOriginal->text(), Qt::ISODate);

    if (!newDatetime.isValid()) {
        QMessageBox::warning(this, "ERROR", "Invalid DateTime");
        return false;
    }

    if (newDatetime == oldDatetime) {
        // Same as old go out!
        return true;
    }

    if (m_exiv2->setDatetime(newDatetime, QExiv2::XmpOriginal)) {
        return m_exiv2->save();
    }
    return false;
}

void MetadataDateTimeDialog::accept()
{
	for (int i = 0; i < m_fileList.count(); i++) {
        if (m_exiv2->load(m_fileList.at(i))) {
            saveData();
        }
	}
	QDialog::accept();
}

void MetadataDateTimeDialog::reject()
{
	emit rejected();
	QDialog::reject();
}
