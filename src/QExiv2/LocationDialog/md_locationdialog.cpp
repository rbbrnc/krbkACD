#include "md_locationdialog.h"
#include "ui_md_locationdialog.h"

#include <QDebug>

#include "iso_countries.h"
#include "QExiv2.h"
#include "geocoding.h"

LocationDialog::LocationDialog(const QStringList &files, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::LocationDialog),
    m_fileList(files),
    m_exiv2(0)
{
	ui->setupUi(this);

    if (m_fileList.isEmpty()) {
		ui->messageLabel->setText("Empty selection!");
        enableWidgets(false);
        return;
    }

    ui->lcRegionCombo->insertItems(0, IsoCountries::instance().worldRegions());
    ui->lcRegionCombo->setCurrentIndex(static_cast<int>(IsoCountries::Europe));
    ui->lsRegionCombo->insertItems(0, IsoCountries::instance().worldRegions());
    ui->lsRegionCombo->setCurrentIndex(static_cast<int>(IsoCountries::Europe));

    m_exiv2 = new QExiv2();

    if (m_fileList.count() == 1) {
        ui->overwriteLc->setChecked(true);
        ui->overwriteLc->hide();
        ui->overwriteLs->setChecked(true);
        ui->overwriteLs->hide();

        if (!getLocations(m_fileList.first())) {
            ui->messageLabel->setText("Cannot Set location on this file: " + m_fileList.first());
            enableWidgets(false);
		} else {
            ui->messageLabel->setText(m_fileList.first());

            ui->lcRegion->setText(m_created.worldRegion());
            ui->lcCountry->setText(m_created.countryName());
            ui->lcCountryCode->setText(m_created.countryCode());
            ui->lcState->setText(m_created.provinceState());
            ui->lcCity->setText(m_created.city());
            ui->lcSublocation->setText(m_created.sublocation());

            ui->lsRegion->setText(m_shown.worldRegion());
            ui->lsCountry->setText(m_shown.countryName());
            ui->lsCountryCode->setText(m_shown.countryCode());
            ui->lsState->setText(m_shown.provinceState());
            ui->lsCity->setText(m_shown.city());
            ui->lsSublocation->setText(m_shown.sublocation());
            if (m_gpsCreated.isValid()) {
                ui->gpsGeoLocation->setText(m_gpsCreated.toString());
            } else {
                ui->geolocationButton->setEnabled(false);
            }
		}
	} else {
		ui->messageLabel->setText("Edit location on multiple files");
		// Multiple selection
		// TODO check values !!
		// Overwrite for now
        ui->overwriteLc->setChecked(true);
        ui->overwriteLc->hide();
        ui->overwriteLs->setChecked(true);
        ui->overwriteLs->hide();
	}

    m_geocoding = new GeoCoding(this);
    connect(m_geocoding, SIGNAL(reverseGeocodeFinished()), this, SLOT(onReverseGeocodeFinished()));
}

LocationDialog::~LocationDialog()
{
    if (m_exiv2) {
        delete m_exiv2;
    }
	delete ui;
}

void LocationDialog::enableWidgets(bool enable)
{
    ui->lcSetButton->setEnabled(enable);
    ui->lcRegion->setEnabled(enable);
    ui->lcCountry->setEnabled(enable);
    ui->lcCountryCode->setEnabled(enable);
    ui->lcState->setEnabled(enable);
    ui->lcCity->setEnabled(enable);
    ui->lcSublocation->setEnabled(enable);

    ui->lsSetButton->setEnabled(enable);
    ui->lsRegion->setEnabled(enable);
    ui->lsCountry->setEnabled(enable);
    ui->lsCountryCode->setEnabled(enable);
    ui->lsState->setEnabled(enable);
    ui->lsCity->setEnabled(enable);
    ui->lsSublocation->setEnabled(enable);

    ui->geolocationButton->setEnabled(enable);
}


bool LocationDialog::getLocations(const QString &file)
{
    if (m_exiv2->load(file)) {
        m_exiv2->locationCreated(m_created, 1);
        m_exiv2->locationShown(m_shown, 1);
        m_gpsCreated = m_exiv2->geoCoordinate();
        return true;
	}
    return false;
}

bool LocationDialog::setLocation(const QString &file)
{
    if (m_exiv2->load(file)) {
        m_exiv2->setLocationCreated(m_created, 1);
        m_exiv2->setLocationShown(m_shown, 1);
        return m_exiv2->save();
    }
    return false;
}

void LocationDialog::on_lcCountryCombo_currentIndexChanged(QString sel)
{
    ui->lcCountryCodeCombo->clear();
    IsoCountries::WorldRegion wr = static_cast<IsoCountries::WorldRegion>(ui->lcRegionCombo->currentIndex());
    ui->lcCountryCodeCombo->insertItems(0, IsoCountries::instance().countryCodes(wr));

    int idx = ui->lcCountryCombo->findText(sel);
    ui->lcCountryCodeCombo->setCurrentIndex(idx);
}

void LocationDialog::on_lsCountryCombo_currentIndexChanged(QString sel)
{
    ui->lsCountryCodeCombo->clear();
    IsoCountries::WorldRegion wr = static_cast<IsoCountries::WorldRegion>(ui->lsRegionCombo->currentIndex());
    ui->lsCountryCodeCombo->insertItems(0, IsoCountries::instance().countryCodes(wr));

    int idx = ui->lsCountryCombo->findText(sel);
    ui->lsCountryCodeCombo->setCurrentIndex(idx);
}

void LocationDialog::updateCountryNameComboBox(QComboBox *cb, int idx)
{
	cb->clear();
	cb->insertItems(0, IsoCountries::instance().countryNames(static_cast<IsoCountries::WorldRegion>(idx)));
}

void LocationDialog::on_lcRegionCombo_currentIndexChanged(int index)
{
    updateCountryNameComboBox(ui->lcCountryCombo, index);
}

void LocationDialog::on_lsRegionCombo_currentIndexChanged(int index)
{
    updateCountryNameComboBox(ui->lsCountryCombo, index);
}

void LocationDialog::on_lcSetButton_clicked()
{
    ui->lcRegion->setText(ui->lcRegionCombo->currentText());
    ui->lcCountry->setText(ui->lcCountryCombo->currentText());
    ui->lcCountryCode->setText(ui->lcCountryCodeCombo->currentText());
}

void LocationDialog::on_lsSetButton_clicked()
{
    ui->lsRegion->setText(ui->lsRegionCombo->currentText());
    ui->lsCountry->setText(ui->lsCountryCombo->currentText());
    ui->lsCountryCode->setText(ui->lsCountryCodeCombo->currentText());
}

// [SLOT public]
void LocationDialog::reject()
{
	emit rejected();
	QDialog::reject();
}

// [SLOT public]
void LocationDialog::accept()
{
    m_created.setWorldRegion(ui->lcRegion->text());
    m_created.setCountryName(ui->lcCountry->text());
    m_created.setCountryCode(ui->lcCountryCode->text());
    m_created.setProvinceState(ui->lcState->text());
    m_created.setCity(ui->lcCity->text());
    m_created.setSublocation(ui->lcSublocation->text());

    m_shown.setWorldRegion(ui->lsRegion->text());
    m_shown.setCountryName(ui->lsCountry->text());
    m_shown.setCountryCode(ui->lsCountryCode->text());
    m_shown.setProvinceState(ui->lsState->text());
    m_shown.setCity(ui->lsCity->text());
    m_shown.setSublocation(ui->lsSublocation->text());

	for (int i = 0; i < m_fileList.count(); i++) {
		setLocation(m_fileList.at(i));
	}
	QDialog::accept();
}
void LocationDialog::onReverseGeocodeFinished()
{
    QGeoAddress addr = m_geocoding->location().address();

    ui->geoLocationOutputs->setPlainText(addr.text());
    //ui->lcRegion->setText(m_created.worldRegion());
    ui->lcCountry->setText(addr.country());
    ui->lcCountryCode->setText(addr.countryCode());
    ui->lcState->setText(addr.state());
    ui->lcCity->setText(addr.city());
    ui->lcSublocation->setText(addr.street());

    enableWidgets(true);
    ui->buttonBox->setEnabled(true);
}

void LocationDialog::on_geolocationButton_clicked()
{
    enableWidgets(false);
    ui->buttonBox->setEnabled(false);
    m_geocoding->reverseGeoCode(m_gpsCreated);
}
