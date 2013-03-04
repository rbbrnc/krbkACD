#include "MetadataEditForm.h"
#include "ui_MetadataEditForm.h"

#include <QMessageBox>
#include <QDebug>

MetadataEditForm::MetadataEditForm(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::MetadataEditForm),
	m_metadataChanged(false),
	m_d(0)
{
	// Init resources on static libraries
	Q_INIT_RESOURCE(SpotlightWidget);

	ui->setupUi(this);


	// Xmp.dc.subject Init
	m_xmpDcSubjectModel  = new QStringListModel(this);
	m_xmpDcSubjectFilter = new QSortFilterProxyModel(this);

	connect(ui->xmpDcSubjectKeyword, SIGNAL(textChanged(QString)),
		m_xmpDcSubjectFilter, SLOT(setFilterFixedString(QString)));

	m_xmpDcSubjectFilter->setFilterCaseSensitivity(Qt::CaseInsensitive);
	ui->xmpDcSubjectListView->setModel(m_xmpDcSubjectFilter);

	connect(ui->addXmpDcSubjectButton, SIGNAL(clicked()),
		this, SLOT(addXmpDcSubject()));

	connect(ui->removeXmpDcSubjectButton, SIGNAL(clicked()),
		this, SLOT(removeXmpDcSubject()));

	connect(ui->xmpDcSubjectListView, SIGNAL(doubleClicked(const QModelIndex &)),
		this, SLOT(xmpDcSubjectDoubleClicked(const QModelIndex &)));

	// Connect LangAlt QLineEdit
	connect(ui->Xmp_iptcExt_Event, SIGNAL(textChanged(const QString &)), this, SLOT(updateLangAltString(const QString &)));
	connect(ui->Xmp_dc_title, SIGNAL(textChanged(const QString &)), this, SLOT(updateLangAltString(const QString &)));

	// Connect LangAlt QPlainTextEdit
	connect(ui->Xmp_dc_description, SIGNAL(textChanged()), this, SLOT(updateLangAltText()));
	connect(ui->imageComment, SIGNAL(textChanged()), this, SLOT(updateImageComment()));


}

MetadataEditForm::~MetadataEditForm()
{
	if (m_d) {
		delete m_d;
	}

	delete m_xmpDcSubjectModel;
	delete m_xmpDcSubjectFilter;

	delete ui;
}

void MetadataEditForm::setFile(const QString &file)
{
	m_metadataChanged = false;
	if (m_d) {
		delete m_d;
		m_d = 0;
	}

	m_d = new QExiv2();

	if (!m_d) {
		return;
	}

	m_d->load(file);
	if (!m_d->isValid()) {
		return;
	}

	updateMetadata();
	updateLocations();
}

bool MetadataEditForm::save()
{
	if (m_d && m_metadataChanged) {
		if (m_d->save()) {
			m_metadataChanged = false;
			return true;
		}
	}
	return false;
}

void MetadataEditForm::updateMetadata()
{
	// Xmp.xmp
	ui->ratingSpinBox->setValue(m_d->xmpTagString("Xmp.xmp.Rating", true).toDouble());

	// [Dublin Core] Xmp.dc
	ui->Xmp_dc_description->setPlainText(m_d->xmpTagStringLangAlt("Xmp.dc.description", QString(), false));
	ui->Xmp_dc_title->setText(m_d->xmpTagStringLangAlt("Xmp.dc.title", QString(), false));

	// dc.subject
	QStringList data;
	data = m_d->xmpTagStringBag("Xmp.dc.subject", true);
	m_xmpDcSubjectModel->setStringList(data);
	m_xmpDcSubjectFilter->setSourceModel(m_xmpDcSubjectModel);


	// IPTC XMP Extension
	ui->Xmp_iptcExt_Event->setText(m_d->xmpTagStringLangAlt("Xmp.iptcExt.Event", QString(), false));
//	ui->exifDateTimeOriginal->setText(m_d->exifTagDateTime("Exif.Photo.DateTimeOriginal").toString());


	// Image Comment
	if (m_d->hasComment()) {
		ui->imageComment->setPlainText(m_d->imgComment());
	}
	if (!m_d->isImgCommentWritable()) {
		ui->imageComment->setReadOnly(true);
		ui->imageCommentLabel->setText("Comment (ReadOnly)");
	}
}

// Add an xmp.dc.subject keyword to the list
void MetadataEditForm::addXmpDcSubject()
{
	QString str = ui->xmpDcSubjectKeyword->text();
	if (str.isEmpty())
		return;

	str = str.simplified();

	m_xmpDcSubjectModel->insertRows(m_xmpDcSubjectModel->rowCount(), 1);
	if (m_xmpDcSubjectModel->setData(m_xmpDcSubjectModel->index(m_xmpDcSubjectModel->rowCount() - 1), str)) {
		if (m_d->setXmpTagStringBag("Xmp.dc.subject", m_xmpDcSubjectModel->stringList())) {
			m_metadataChanged = true;
		} else {
			showErrorMessage(tr("Cannot set Xmp.dc.subject"));
		}
	}

	ui->xmpDcSubjectKeyword->clear();
}

// Remove an xmp.dc.subject keyword from the list
void MetadataEditForm::removeXmpDcSubject()
{
	QModelIndex idx = ui->xmpDcSubjectListView->currentIndex();
	if (m_xmpDcSubjectModel->removeRow(idx.row())) {
		if (!m_d->setXmpTagStringBag("Xmp.dc.subject", m_xmpDcSubjectModel->stringList())) {
			m_metadataChanged = true;
		} else {
			showErrorMessage(tr("Cannot set Xmp.dc.subject"));
		}
	}
}

void MetadataEditForm::xmpDcSubjectDoubleClicked(const QModelIndex &index)
{
	ui->xmpDcSubjectKeyword->setText(index.data().toString());
}

void MetadataEditForm::updateImageComment()
{
	if (m_d->setImgComment(ui->imageComment->toPlainText().toLatin1())) {
		m_metadataChanged = true;
	} else {
		showErrorMessage(tr("Cannot set Image Comment"));
	}
}

void MetadataEditForm::updateLangAltString(const QString &text)
{
	// Get the tag to change (use object name to store the tag)
	QString tag = sender()->objectName().replace('_', '.');

	if (m_d->setXmpTagStringLangAlt(tag.toLatin1(), text, QString())) {
		m_metadataChanged = true;
	} else {
		showErrorMessage(tr("Cannot set %1").arg(tag));
	}
}

void MetadataEditForm::updateLangAltText()
{
	QPlainTextEdit *l = static_cast<QPlainTextEdit *>(sender());

	// Get the tag to change (use object name to store the tag)
	QString tag = l->objectName().replace('_', '.');

	if (m_d->setXmpTagStringLangAlt(tag.toLatin1(), l->toPlainText(), QString())) {
		m_metadataChanged = true;
	} else {
		showErrorMessage(tr("Cannot set %1").arg(tag));
	}
}

/*
void MetadataEdit::on_ratingSpinBox_valueChanged(double value)
{
	if (!m_d->setXmpTagString("Xmp.xmp.Rating", QString::number(value, 'f', 1))) {
		QMessageBox::critical(this, tr("Error"), tr("Cannot set Xmp.xmp.Rating"), QMessageBox::Abort);
	}
}
*/

void MetadataEditForm::showErrorMessage(const QString &text)
{
	QMessageBox::critical(this, tr("Error"), text, QMessageBox::Abort);
}



void MetadataEditForm::updateLocations()
{
	// Location Created
	// This information describes the location where the image was created,
	// the location of the camera during shot creation.
	// The typical case is when a GPS receiver injects the current location
	// into an image at shooting time (camera location).

	//
	// Exif.GPSInfo.GPSLatitudeRef	(Ascii)
	// Exif.GPSInfo.GPSLatitude	(Rational)
	// XMP:GPSLatitude
	//
	//
	// Exif.GPSInfo.GPSLatitudeRef:
	//	Indicates whether the latitude is north or south latitude.
	//	The ASCII value 'N' indicates north latitude, and 'S' is south latitude.
	//
	// Exif.GPSInfo.GPSLatitude:
	//	Indicates the latitude.
	//	The latitude is expressed as three RATIONAL values giving the degrees, minutes, and seconds, respectively.
	//	When degrees, minutes and seconds are expressed, the format is dd/1,mm/1,ss/1.
	//	When degrees and minutes are used and, for example, fractions of minutes are given up to two decimal places,
	//	the format is dd/1,mmmm/100,0/1.
	//
	// XMP:GPSLatitude
	ui->lcLatitude->setText(m_d->exifTagString("Exif.GPSInfo.GPSLatitude", false));

	// EXIF:GPSLongitude
	// EXIF:GPSLongitudeRef
	// XMP:GPSLongitude
	ui->lcLongitude->setText(m_d->exifTagString("Exif.GPSInfo.GPSLongitude", false));

	// EXIF:GPSAltitude
	// EXIF:GPSAltitudeRef
	// XMP:GPSAltitude
	// XMP:GPSAltitudeRef
	ui->lcAltitude->setText(m_d->exifTagString("Exif.GPSInfo.GPSAltitude", false));

	// EXIF:GPSImgDirection
	// EXIF:GPSImgDirectionRef
	// XMP:GPSImgDirection
	// XMP:GPSImgDirectionRef
	ui->lcDirection->setText("");

	// EXIF:GPSMapDatum
	// XMP:GPSMapDatum
	ui->lcMapDatum->setText("");

	// XMP:LocationCreatedWorldRegion
	// XMP:LocationCreatedCountryCode
	// XMP:LocationCreatedCountryName
	// XMP:LocationCreatedProvinceState
	// XMP:LocationCreatedCity
	// XMP:LocationCreatedSublocation
	if (!m_d->xmpTagString("Xmp.iptcExt.LocationCreated", false).isNull()) {
		ui->lcWorldRegion->setText(m_d->xmpTagString("Xmp.iptcExt.LocationCreated[1]/Iptc4xmpExt:WorldRegion", false));
		ui->lcCountryCode->setText(m_d->xmpTagString("Xmp.iptcExt.LocationCreated[1]/Iptc4xmpExt:CountryCode", false));
		ui->lcCountryName->setText(m_d->xmpTagString("Xmp.iptcExt.LocationCreated[1]/Iptc4xmpExt:CountryName", false));
		ui->lcProvinceState->setText(m_d->xmpTagString("Xmp.iptcExt.LocationCreated[1]/Iptc4xmpExt:ProvinceState", false));
		ui->lcCity->setText(m_d->xmpTagString("Xmp.iptcExt.LocationCreated[1]/Iptc4xmpExt:City", false));
		ui->lcSublocation->setText(m_d->xmpTagString("Xmp.iptcExt.LocationCreated[1]/Iptc4xmpExt:Sublocation", false));
	}

	// Location Shown
	// This information describes the location of the main subject being shown
	// in an image. For example, a picture of Mount Fuji would be tagged with
	// the coordinates of where the mountain is located (subject location),
	// although the picture may have been taken from downtown Tokyo.

	ui->lsLatitude->setText("");
	ui->lsLongitude->setText("");
	ui->lsAltitude->setText("");
	ui->lsDirection->setText("");
	ui->lsMapDatum->setText("");

	// XMP:LocationShownWorldRegion
	// XMP:LocationCreatedCountryCode
	// XMP:LocationCreatedCountryName
	// XMP:LocationCreatedProvinceState
	// XMP:LocationCreatedCity
	// XMP:LocationCreatedSublocation
	if (!m_d->xmpTagString("Xmp.iptcExt.LocationShown", false).isNull()) {
		ui->lsWorldRegion->setText(m_d->xmpTagString("Xmp.iptcExt.LocationShown[1]/Iptc4xmpExt:WorldRegion", false));
		ui->lsCountryCode->setText(m_d->xmpTagString("Xmp.iptcExt.LocationShown[1]/Iptc4xmpExt:CountryCode", false));
		ui->lsCountryName->setText(m_d->xmpTagString("Xmp.iptcExt.LocationShown[1]/Iptc4xmpExt:CountryName", false));
		ui->lsProvinceState->setText(m_d->xmpTagString("Xmp.iptcExt.LocationShown[1]/Iptc4xmpExt:ProvinceState", false));
		ui->lsCity->setText(m_d->xmpTagString("Xmp.iptcExt.LocationShown[1]/Iptc4xmpExt:City", false));
		ui->lsSublocation->setText(m_d->xmpTagString("Xmp.iptcExt.LocationShown[1]/Iptc4xmpExt:Sublocation", false));
	}
}

