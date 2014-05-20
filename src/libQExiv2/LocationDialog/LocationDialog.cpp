#include "LocationDialog.h"
#include "ui_LocationDialog.h"

#include <QDebug>

#include "iso_countries.h"
#include "QExiv2.h"

LocationDialog::LocationDialog(const QStringList &files, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::LocationDialog),
	m_fileList(files)
{
	ui->setupUi(this);

	ui->lcWorldRegionComboBox->insertItems(0, IsoCountries::instance().worldRegions());
	ui->lcWorldRegionComboBox->setCurrentIndex(static_cast<int>(IsoCountries::Europe));
	ui->lsWorldRegionComboBox->insertItems(0, IsoCountries::instance().worldRegions());
	ui->lsWorldRegionComboBox->setCurrentIndex(static_cast<int>(IsoCountries::Europe));

	if (m_fileList.count() == 0) {
		ui->messageLabel->setText("Empty selection!");
	} else if (m_fileList.count() == 1) {
		ui->lcOverwriteLocations->setChecked(true);
		ui->lcOverwriteLocations->hide();
		ui->lsOverwriteLocations->setChecked(true);
		ui->lsOverwriteLocations->hide();

		if (!getLocations(m_fileList.at(0))) {
			ui->messageLabel->setText("Cannot Set location on this file: " + m_fileList.at(0));

			ui->lcSetButton->setEnabled(false);
			ui->lcWorldRegion->setEnabled(false);
			ui->lcCountryName->setEnabled(false);
			ui->lcCountryCode->setEnabled(false);
			ui->lcProvinceState->setEnabled(false);
			ui->lcCity->setEnabled(false);
			ui->lcSublocation->setEnabled(false);

			ui->lsSetButton->setEnabled(false);
			ui->lsWorldRegion->setEnabled(false);
			ui->lsCountryName->setEnabled(false);
			ui->lsCountryCode->setEnabled(false);
			ui->lsProvinceState->setEnabled(false);
			ui->lsCity->setEnabled(false);
			ui->lsSublocation->setEnabled(false);
		} else {
			ui->messageLabel->setText(m_fileList.at(0));

			ui->lcWorldRegion->setText(m_locationCreated.worldRegion);
			ui->lcCountryName->setText(m_locationCreated.countryName);
			ui->lcCountryCode->setText(m_locationCreated.countryCode);
			ui->lcProvinceState->setText(m_locationCreated.provinceState);
			ui->lcCity->setText(m_locationCreated.city);
			ui->lcSublocation->setText(m_locationCreated.sublocation);

			ui->lsWorldRegion->setText(m_locationShown.worldRegion);
			ui->lsCountryName->setText(m_locationShown.countryName);
			ui->lsCountryCode->setText(m_locationShown.countryCode);
			ui->lsProvinceState->setText(m_locationShown.provinceState);
			ui->lsCity->setText(m_locationShown.city);
			ui->lsSublocation->setText(m_locationShown.sublocation);
		}
	} else {
		ui->messageLabel->setText("Edit location on multiple files");
		// Multiple selection
		// TODO check values !!
		// Overwrite for now
		ui->lcOverwriteLocations->setChecked(true);
		ui->lcOverwriteLocations->hide();
		ui->lsOverwriteLocations->setChecked(true);
		ui->lsOverwriteLocations->hide();
	}
}

LocationDialog::~LocationDialog()
{
	delete ui;
}

bool LocationDialog::getLocations(const QString &file)
{
	QExiv2 *e = new QExiv2();
    bool rc = e->load(file);

    if (rc) {
        e->locationCreated(m_locationCreated, 1);
        e->locationShown(m_locationShown, 1);
	}

    delete e;
    return rc;
}

bool LocationDialog::setLocation(const QString &file)
{
	QExiv2 *e = new QExiv2();
	if (!e->load(file)) {
		qDebug() << __PRETTY_FUNCTION__ << "Skip file:" << file;
		delete e;
		return false;
	}

    e->setLocationCreated(m_locationCreated, 1);
    e->setLocationShown(m_locationShown, 1);

    bool rc = e->save();
    if (!rc) {
        qDebug() << __PRETTY_FUNCTION__ << "Error set Xmp Location on:" << file;
	}

	delete e;
    return rc;
}

void LocationDialog::on_lcCountryNameComboBox_currentIndexChanged(QString sel)
{
	ui->lcCountryCodeComboBox->clear();
	IsoCountries::WorldRegion wr = static_cast<IsoCountries::WorldRegion>(ui->lcWorldRegionComboBox->currentIndex());
	ui->lcCountryCodeComboBox->insertItems(0, IsoCountries::instance().countryCodes(wr));

	int idx = ui->lcCountryNameComboBox->findText(sel);
	ui->lcCountryCodeComboBox->setCurrentIndex(idx);
}

void LocationDialog::on_lsCountryNameComboBox_currentIndexChanged(QString sel)
{
	ui->lsCountryCodeComboBox->clear();
	IsoCountries::WorldRegion wr = static_cast<IsoCountries::WorldRegion>(ui->lsWorldRegionComboBox->currentIndex());
	ui->lsCountryCodeComboBox->insertItems(0, IsoCountries::instance().countryCodes(wr));

	int idx = ui->lsCountryNameComboBox->findText(sel);
	ui->lsCountryCodeComboBox->setCurrentIndex(idx);
}

void LocationDialog::updateCountryNameComboBox(QComboBox *cb, int idx)
{
	cb->clear();
	cb->insertItems(0, IsoCountries::instance().countryNames(static_cast<IsoCountries::WorldRegion>(idx)));
}

void LocationDialog::on_lcWorldRegionComboBox_currentIndexChanged(int index)
{
	updateCountryNameComboBox(ui->lcCountryNameComboBox, index);
}

void LocationDialog::on_lsWorldRegionComboBox_currentIndexChanged(int index)
{
	updateCountryNameComboBox(ui->lsCountryNameComboBox, index);
}

void LocationDialog::on_lcSetButton_clicked()
{
	ui->lcWorldRegion->setText(ui->lcWorldRegionComboBox->currentText());
	ui->lcCountryName->setText(ui->lcCountryNameComboBox->currentText());
	ui->lcCountryCode->setText(ui->lcCountryCodeComboBox->currentText());
}

void LocationDialog::on_lsSetButton_clicked()
{
	ui->lsWorldRegion->setText(ui->lsWorldRegionComboBox->currentText());
	ui->lsCountryName->setText(ui->lsCountryNameComboBox->currentText());
	ui->lsCountryCode->setText(ui->lsCountryCodeComboBox->currentText());
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
	m_locationCreated.worldRegion   = ui->lcWorldRegion->text();
	m_locationCreated.countryName   = ui->lcCountryName->text();
	m_locationCreated.countryCode   = ui->lcCountryCode->text();
	m_locationCreated.provinceState = ui->lcProvinceState->text();
	m_locationCreated.city          = ui->lcCity->text();
	m_locationCreated.sublocation   = ui->lcSublocation->text();

	m_locationShown.worldRegion   = ui->lsWorldRegion->text();
	m_locationShown.countryName   = ui->lsCountryName->text();
	m_locationShown.countryCode   = ui->lsCountryCode->text();
	m_locationShown.provinceState = ui->lsProvinceState->text();
	m_locationShown.city          = ui->lsCity->text();
	m_locationShown.sublocation   = ui->lsSublocation->text();

	for (int i = 0; i < m_fileList.count(); i++) {
		setLocation(m_fileList.at(i));
	}
	QDialog::accept();
}

