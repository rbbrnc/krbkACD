#include "LocationDialog.h"
#include "ui_LocationDialog.h"

#include <QDebug>

#include "iso_countries.h"
#include "QExiv2.h"

bool Location::isEmpty() const
{
	return (worldRegion.isEmpty()
		&& countryName.isEmpty()
		&& countryCode.isEmpty()
		&& provinceState.isEmpty()
		&& city.isEmpty()
	        && sublocation.isEmpty());
}



LocationDialog::LocationDialog(const QStringList &files, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::LocationDialog),
	m_fileList(files)
{
	ui->setupUi(this);

	IsoCountries *ic = IsoCountries::instance();

	ui->lcWorldRegionComboBox->insertItems(0, ic->worldRegions());
	ui->lcWorldRegionComboBox->setCurrentIndex(static_cast<int>(IsoCountries::Europe));
	ui->lsWorldRegionComboBox->insertItems(0, ic->worldRegions());
	ui->lsWorldRegionComboBox->setCurrentIndex(static_cast<int>(IsoCountries::Europe));

	if (m_fileList.count() == 1) {
		ui->lcOverwriteLocations->setChecked(true);
		ui->lcOverwriteLocations->hide();
		ui->lsOverwriteLocations->setChecked(true);
		ui->lsOverwriteLocations->hide();

		if (!getLocations(m_fileList.at(0))) {
			ui->messageLabel->setText("Cannot Set loction on this file: " + m_fileList.at(0));

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
	}
}

LocationDialog::~LocationDialog()
{
	delete ui;
}

bool LocationDialog::getLocations(const QString &file)
{
	QExiv2 *e = new QExiv2();
	if (!e->load(file)) {
		delete e;
		return false;
	}

	// ===================================================================
	// Get Location Created tags
	//
	// This information describes the location where the image was created,
	// the location of the camera during shot creation.
	// The typical case is when a GPS receiver injects the current location
	// into an image at shooting time (camera location).
	//
	// 1st try Xmp.iptcExt tags
	//
	// Xmp.iptcExt.LocationCreated[1]/Iptc4xmpExt:WorldRegion
	// Xmp.iptcExt.LocationCreated[1]/Iptc4xmpExt:CountryName
	// Xmp.iptcExt.LocationCreated[1]/Iptc4xmpExt:CountryCode
	// Xmp.iptcExt.LocationCreated[1]/Iptc4xmpExt:City
	// Xmp.iptcExt.LocationCreated[1]/Iptc4xmpExt:ProvinceState
	// Xmp.iptcExt.LocationCreated[1]/Iptc4xmpExt:Sublocation
	//
	// 2nd Try Xmp.photoshop and Iptc.Application2 tags
	//
	// Xmp.photoshop.Country _or_  Iptc.Application2.CountryName
	// ---                         Iptc.Application2.CountryCode
	// Xmp.photoshop.State   _or_  Iptc.Application2.ProvinceState
	// Xmp.photoshop.City    _or_  Iptc.Application2.City
	// ---                         Iptc.Application2.SubLocation
	//
	// ===================================================================
	m_locationCreated.worldRegion = e->xmpTagString("Xmp.iptcExt.LocationCreated[1]/Iptc4xmpExt:WorldRegion", true);
	m_locationCreated.countryName = e->xmpTagString("Xmp.iptcExt.LocationCreated[1]/Iptc4xmpExt:CountryName", true);
	if (m_locationCreated.countryName.isEmpty()) {
		m_locationCreated.countryName = e->xmpTagString("Xmp.photoshop.Country", true);
		if (m_locationCreated.countryName.isEmpty()) {
			m_locationCreated.countryName = e->iptcTagString("Iptc.Application2.CountryName", true);
		}
	}

	m_locationCreated.countryCode = e->xmpTagString("Xmp.iptcExt.LocationCreated[1]/Iptc4xmpExt:CountryCode", true);
	if (m_locationCreated.countryCode.isEmpty()) {
		m_locationCreated.countryCode = e->iptcTagString("Iptc.Application2.CountryCode", true);
	}

	m_locationCreated.provinceState = e->xmpTagString("Xmp.iptcExt.LocationCreated[1]/Iptc4xmpExt:ProvinceState", true);
	if (m_locationCreated.provinceState.isEmpty()) {
		m_locationCreated.provinceState = e->xmpTagString("Xmp.photoshop.State", true);
		if (m_locationCreated.provinceState.isEmpty()) {
			m_locationCreated.provinceState = e->iptcTagString("Iptc.Application2.ProvinceState", true);
		}
	}

	m_locationCreated.city = e->xmpTagString("Xmp.iptcExt.LocationCreated[1]/Iptc4xmpExt:City", true);
	if (m_locationCreated.city.isEmpty()) {
		m_locationCreated.city = e->xmpTagString("Xmp.photoshop.City", true);
		if (m_locationCreated.city.isEmpty()) {
			m_locationCreated.city = e->iptcTagString("Iptc.Application2.City", true);
		}
	}

	m_locationCreated.sublocation = e->xmpTagString("Xmp.iptcExt.LocationCreated[1]/Iptc4xmpExt:Sublocation", true);
	if (m_locationCreated.sublocation.isEmpty()) {
		m_locationCreated.sublocation = e->iptcTagString("Iptc.Application2.SubLocation", true);
	}

	// ===================================================================
	// Get Location Shown tags
	//
	// This information describes the location of the main subject being
	// shown in an image. For example, a picture of Mount Fuji would be
	// tagged with the coordinates of where the mountain is located
	// (subject location), although the picture may have been taken from
	// downtown Tokyo.
	//
	// 1st try Xmp.iptcExt tags
	//
	// Xmp.iptcExt.LocationShown[1]/Iptc4xmpExt:WorldRegion
	// Xmp.iptcExt.LocationShown[1]/Iptc4xmpExt:CountryName
	// Xmp.iptcExt.LocationShown[1]/Iptc4xmpExt:CountryCode
	// Xmp.iptcExt.LocationShown[1]/Iptc4xmpExt:ProvinceState
	// Xmp.iptcExt.LocationShown[1]/Iptc4xmpExt:City
	// Xmp.iptcExt.LocationShown[1]/Iptc4xmpExt:Sublocation
	//
	// 2nd try legacy Iptc.Application2 tags
	//
	// Iptc.Application2.LocationName
	// Iptc.Application2.LocationCode
	//
	// ====================================================================
	m_locationShown.worldRegion = e->xmpTagString("Xmp.iptcExt.LocationShown[1]/Iptc4xmpExt:WorldRegion", true);
	m_locationShown.countryName = e->xmpTagString("Xmp.iptcExt.LocationShown[1]/Iptc4xmpExt:CountryName", true);
	if (m_locationShown.countryName.isEmpty()) {
		m_locationShown.countryName = e->iptcTagString("Iptc.Application2.LocationName", true);
	}

	m_locationShown.countryCode = e->xmpTagString("Xmp.iptcExt.LocationShown[1]/Iptc4xmpExt:CountryCode", true);
	if (m_locationShown.countryCode.isEmpty()) {
		m_locationShown.countryCode = e->iptcTagString("Iptc.Application2.LocationCode", true);
	}

	m_locationShown.provinceState = e->xmpTagString("Xmp.iptcExt.LocationShown[1]/Iptc4xmpExt:ProvinceState", true);
	m_locationShown.city          = e->xmpTagString("Xmp.iptcExt.LocationShown[1]/Iptc4xmpExt:City", true);
	m_locationShown.sublocation   = e->xmpTagString("Xmp.iptcExt.LocationShown[1]/Iptc4xmpExt:Sublocation", true);

	delete e;
	return true;
}

bool LocationDialog::setLocation(const QString &file)
{
	QExiv2 *e = new QExiv2();
	if (!e->load(file)) {
		qDebug() << __PRETTY_FUNCTION__ << "Skip file:" << file;
		delete e;
		return false;
	}

	if (m_locationCreated.isEmpty()) {
		e->removeXmpBag("Xmp.iptcExt.LocationCreated", 27);
	} else {
		if (e->xmpTagString("Xmp.iptcExt.LocationCreated").isNull()) {
			e->setXmpTagBag("Xmp.iptcExt.LocationCreated");
		}
		e->setXmpTagString("Xmp.iptcExt.LocationCreated[1]/Iptc4xmpExt:WorldRegion", m_locationCreated.worldRegion);
		e->setXmpTagString("Xmp.iptcExt.LocationCreated[1]/Iptc4xmpExt:CountryName", m_locationCreated.countryName);
		e->setXmpTagString("Xmp.iptcExt.LocationCreated[1]/Iptc4xmpExt:CountryCode", m_locationCreated.countryCode);
		e->setXmpTagString("Xmp.iptcExt.LocationCreated[1]/Iptc4xmpExt:ProvinceState", m_locationCreated.provinceState);
		e->setXmpTagString("Xmp.iptcExt.LocationCreated[1]/Iptc4xmpExt:City", m_locationCreated.city);
		e->setXmpTagString("Xmp.iptcExt.LocationCreated[1]/Iptc4xmpExt:Sublocation", m_locationCreated.sublocation);
	}

	if (m_locationShown.isEmpty()) {
		e->removeXmpBag("Xmp.iptcExt.LocationShown", 25);
	} else {
		if (e->xmpTagString("Xmp.iptcExt.LocationShown").isNull()) {
			e->setXmpTagBag("Xmp.iptcExt.LocationShown");
		}
		e->setXmpTagString("Xmp.iptcExt.LocationShown[1]/Iptc4xmpExt:WorldRegion", m_locationShown.worldRegion);
		e->setXmpTagString("Xmp.iptcExt.LocationShown[1]/Iptc4xmpExt:CountryName", m_locationShown.countryName);
		e->setXmpTagString("Xmp.iptcExt.LocationShown[1]/Iptc4xmpExt:CountryCode", m_locationShown.countryCode);
		e->setXmpTagString("Xmp.iptcExt.LocationShown[1]/Iptc4xmpExt:ProvinceState", m_locationShown.provinceState);
		e->setXmpTagString("Xmp.iptcExt.LocationShown[1]/Iptc4xmpExt:City", m_locationShown.city);
		e->setXmpTagString("Xmp.iptcExt.LocationShown[1]/Iptc4xmpExt:Sublocation", m_locationShown.sublocation);
	}

	if (e->save()) {
		delete e;
		return true;
	}

	qDebug() << __PRETTY_FUNCTION__ << "Error set Xmp Location on file:" << file;
	delete e;
	return false;
}

void LocationDialog::on_lcCountryNameComboBox_currentIndexChanged(QString sel)
{
	ui->lcCountryCodeComboBox->clear();
	IsoCountries *ic = IsoCountries::instance();
	IsoCountries::WorldRegion wr = static_cast<IsoCountries::WorldRegion>(ui->lcWorldRegionComboBox->currentIndex());
	ui->lcCountryCodeComboBox->insertItems(0, ic->countryCodes(wr));

	int idx = ui->lcCountryNameComboBox->findText(sel);
	ui->lcCountryCodeComboBox->setCurrentIndex(idx);
}

void LocationDialog::on_lsCountryNameComboBox_currentIndexChanged(QString sel)
{
	ui->lsCountryCodeComboBox->clear();
	IsoCountries *ic = IsoCountries::instance();
	IsoCountries::WorldRegion wr = static_cast<IsoCountries::WorldRegion>(ui->lsWorldRegionComboBox->currentIndex());
	ui->lsCountryCodeComboBox->insertItems(0, ic->countryCodes(wr));

	int idx = ui->lsCountryNameComboBox->findText(sel);
	ui->lsCountryCodeComboBox->setCurrentIndex(idx);
}

void LocationDialog::updateCountryNameComboBox(QComboBox *cb, int idx)
{
	cb->clear();
	IsoCountries *ic = IsoCountries::instance();
	cb->insertItems(0, ic->countryNames(static_cast<IsoCountries::WorldRegion>(idx)));
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

