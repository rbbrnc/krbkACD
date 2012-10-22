#include "MetadataLocation.h"
#include "ui_MetadataLocation.h"

#include <QtGui>
#include "QExiv2.h"

MetadataLocation::MetadataLocation(QExiv2 *metadata, QWidget *parent)
	: QWidget(parent),
	ui(new Ui::MetadataLocation)
{
	ui->setupUi(this);

	m_md = metadata;

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
	ui->lcLatitude->setText(m_md->exifTagString("Exif.GPSInfo.GPSLatitude", false));

	// EXIF:GPSLongitude
	// EXIF:GPSLongitudeRef
	// XMP:GPSLongitude
	ui->lcLongitude->setText(m_md->exifTagString("Exif.GPSInfo.GPSLongitude", false));

	// EXIF:GPSAltitude
	// EXIF:GPSAltitudeRef
	// XMP:GPSAltitude
	// XMP:GPSAltitudeRef
	ui->lcAltitude->setText(m_md->exifTagString("Exif.GPSInfo.GPSAltitude", false));

	// EXIF:GPSImgDirection
	// EXIF:GPSImgDirectionRef
	// XMP:GPSImgDirection
	// XMP:GPSImgDirectionRef
	ui->lcDirection->setText("");

	// EXIF:GPSMapDatum
	// XMP:GPSMapDatum
	ui->lcMapDatum->setText("");

	// XMP:LocationCreatedWorldRegion
	ui->lcWorldRegion->setText("");

	// XMP:LocationCreatedCountryCode
	ui->lcCountryCode->setText(m_md->xmpTagString("Xmp.iptcExt.CountryCode", false));

	// XMP:LocationCreatedCountryName
	ui->lcCountryName->setText(m_md->xmpTagString("Xmp.iptcExt.LocationCreated[1]/Iptc4xmpExt:CountryName", false));

	// XMP:LocationCreatedProvinceState
	ui->lcProvinceState->setText(m_md->xmpTagString("Xmp.iptcExt.LocationCreated[1]/Iptc4xmpExt:ProvinceState", false));

	// XMP:LocationCreatedCity
	ui->lcCity->setText(m_md->xmpTagString("Xmp.iptcExt.LocationCreated[1]/Iptc4xmpExt:City", false));

	// XMP:LocationCreatedSublocation
	ui->lcSublocation->setText(m_md->xmpTagString("Xmp.iptcExt.LocationCreated[1]/Iptc4xmpExt:Sublocation", false));

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
	ui->lsWorldRegion->setText("");

	ui->lsCountryCode->setText("");

	ui->lsCountryName->setText(m_md->xmpTagString("Xmp.iptcExt.LocationShown[1]/Iptc4xmpExt:CountryName", false));
	ui->lsProvinceState->setText(m_md->xmpTagString("Xmp.iptcExt.LocationShown[1]/Iptc4xmpExt:ProvinceState", false));
	ui->lsCity->setText(m_md->xmpTagString("Xmp.iptcExt.LocationShown[1]/Iptc4xmpExt:City", false));
	ui->lsSublocation->setText(m_md->xmpTagString("Xmp.iptcExt.LocationShown[1]/Iptc4xmpExt:Sublocation", false));
}

