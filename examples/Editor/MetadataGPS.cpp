#include "MetadataGPS.h"
#include "ui_MetadataGPS.h"

#include <QtGui>

MetadataGPS::MetadataGPS(const QFileInfo &fileInfo, QWidget *parent)
	: QWidget(parent),
	ui(new Ui::MetadataGPS)
{
	ui->setupUi(this);

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
	ui->lcLatitude->setText("");

	// EXIF:GPSLongitude
	// EXIF:GPSLongitudeRef
	// XMP:GPSLongitude
	ui->lcLongitude->setText("");

	// EXIF:GPSAltitude
	// EXIF:GPSAltitudeRef
	// XMP:GPSAltitude
	// XMP:GPSAltitudeRef
	ui->lcAltitude->setText("");

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
	ui->lcCountryCode->setText("");

	// XMP:LocationCreatedCountryName
	ui->lcCountryName->setText("");

	// XMP:LocationCreatedProvinceState
	ui->lcProvinceState->setText("");

	// XMP:LocationCreatedCity
	ui->lcCity->setText("");

	// XMP:LocationCreatedSublocation
	ui->lcSublocation->setText("");

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

	ui->lsCountryName->setText("");

	ui->lsProvinceState->setText("");

	ui->lsCity->setText("");

	ui->lsSublocation->setText("");
}

#if 0

Location Shown
World Region
Enter the name of the World Region pictured in this image.  es: The Americas

Country Code
	IPTC:Country-PrimaryLocationCode
	MP:CountryCode
	MP:LocationShownCountryCode
Enter ISO (International Organization for Standardization) Country Code (e.g. Germany-DE, Antarctica-AQ...)

Country
	IPTC:Country-PrimaryLocationName
	XMP:Country
	XMP:LocationShownCountryName 	\u2598 Location Shown: City & State & CountryEnter the names of the City, State/Province, and Country pictured in this image.
\u2192 Country: United States
\u2003 State: Wisconsin
\u2003 City: Juneau
To avoid ambiguity enter either:
1. Country or
2. Country & State/Province, or
3. Country & State/Province & City.
State 	IPTC:Province-State
XMP:State
XMP:LocationShownProvinceState 	\u2598 Location Shown: City & State & CountryEnter the names of the City, State/Province, and Country pictured in this image.
\u2192 Country: United States
\u2003 State: Wisconsin
\u2003 City: Juneau
To avoid ambiguity enter either:
1. Country or
2. Country & State/Province, or
3. Country & State/Province & City.

City
	IPTC:City
	MP:City
	MP:LocationShownCity

Enter the names of the City, State/Province, and Country pictured in this image.
Country: United States
State: Wisconsin
City: Juneau
To avoid ambiguity enter either:
1. Country or
2. Country & State/Province, or
3. Country & State/Province & City.

Sublocation
	IPTC:Sub-location
	XMP:Location
	XMP:LocationShownSublocation

Enter the name of a sublocation to a city or the name of a well known location or (natural) monument outside a city.
example: Denali National Park
Note: Considered the bottom of the top-down geographical heirachy: World Region-> Country Code (optional)-> Country-> State/Province-> City-> Sublocation.

LatitudeLongitude
EXIF:GPSDestLatitude,  EXIF:GPSDestLatitudeRef,  XMP:GPSDestLatitude
EXIF:GPSDestLongitude, EXIF:GPSDestLongitudeRef, XMP:GPSDestLongitude

Entered using manual input of coordinates if the location in the image is different from the location the photo was taken
Longitude -111.946472°
Latitude 33.462195°
Note: "-" indicates West for longitude and South for latitude.

EXIF: GPSDestLongitude & GPSDestLongitudeRef, GPSDestLatitude & GPSDestLatitudeRef


==========================================================================================000

Exif.GPSInfo.GPSVersionID 	Byte 		This tag is mandatory when <GPSInfo> tag is present.
						Indicates the version of <GPSInfoIFD>.
						The version is given as 2.0.0.0.
						(Note: The <GPSVersionID> tag is given in bytes, unlike the <ExifVersion> tag.
						When the version is 2.0.0.0, the tag value is 02000000.H).

Exif.GPSInfo.GPSLatitudeRef 	Ascii 		Indicates whether the latitude is north or south latitude.
						The ASCII value 'N' indicates north latitude, and 'S' is south latitude.

Exif.GPSInfo.GPSLatitude 	Rational 	Indicates the latitude.
						The latitude is expressed as three RATIONAL values giving the degrees, minutes, and seconds, respectively.
						When degrees, minutes and seconds are expressed, the format is dd/1,mm/1,ss/1.
						When degrees and minutes are used and, for example, fractions of minutes are given up to two decimal places,
						the format is dd/1,mmmm/100,0/1.

Exif.GPSInfo.GPSLongitudeRef 	Ascii 		Indicates whether the longitude is east or west longitude.
						ASCII 'E' indicates east longitude, and 'W' is west longitude.

Exif.GPSInfo.GPSLongitude 	Rational 	Indicates the longitude.
						The longitude is expressed as three RATIONAL values giving the degrees, minutes, and seconds, respectively.
						When degrees, minutes and seconds are expressed, the format is ddd/1,mm/1,ss/1.
						When degrees and minutes are used and, for example, fractions of minutes are given up to two decimal places,
						the format is ddd/1,mmmm/100,0/1.

Exif.GPSInfo.GPSAltitudeRef 	Byte 		Indicates the altitude used as the reference altitude.
						If the reference is sea level and the altitude is above sea level, 0 is given.
						If the altitude is below sea level, a value of 1 is given and the altitude is indicated
						as an absolute value in the GSPAltitude tag. The reference unit is meters.
						Note that this tag is BYTE type, unlike other reference tags.

Exif.GPSInfo.GPSAltitude 	Rational 	Indicates the altitude based on the reference in GPSAltitudeRef.
						Altitude is expressed as one RATIONAL value. The reference unit is meters.

Exif.GPSInfo.GPSTimeStamp 	Rational 	Indicates the time as UTC (Coordinated Universal Time).
						<TimeStamp> is expressed as three RATIONAL values giving the hour, minute, and second (atomic clock).

Exif.GPSInfo.GPSSatellites 	Ascii 		Indicates the GPS satellites used for measurements.
						This tag can be used to describe the number of satellites, their ID number,
						angle of elevation, azimuth, SNR and other information in ASCII notation.
						The format is not specified. If the GPS receiver is incapable of taking measurements,
						value of the tag is set to NULL.

Exif.GPSInfo.GPSStatus	 	Ascii 		Indicates the status of the GPS receiver when the image is recorded.
						"A" means measurement is in progress, and "V" means the measurement is Interoperability.

Exif.GPSInfo.GPSMeasureMode 	Ascii 		Indicates the GPS measurement mode.
						"2" means two-dimensional measurement and "3" means three-dimensional measurement is in progress.

Exif.GPSInfo.GPSDOP	 	Rational 	Indicates the GPS DOP (data degree of precision).
						An HDOP value is written during two-dimensional measurement, and PDOP during three-dimensional measurement.

Exif.GPSInfo.GPSSpeedRef 	Ascii 		Indicates the unit used to express the GPS receiver speed of movement.
						"K" "M" and "N" represents kilometers per hour, miles per hour, and knots.

Exif.GPSInfo.GPSSpeed	 	Rational 	Indicates the speed of GPS receiver movement.

Exif.GPSInfo.GPSTrackRef 	Ascii 		Indicates the reference for giving the direction of GPS receiver movement.
						"T" denotes true direction and "M" is magnetic direction.

Exif.GPSInfo.GPSTrack 		Rational 	Indicates the direction of GPS receiver movement. The range of values is from 0.00 to 359.99.

Exif.GPSInfo.GPSImgDirectionRef	Ascii 		Indicates the reference for giving the direction of the image when it is captured.
						"T" denotes true direction and "M" is magnetic direction.

Exif.GPSInfo.GPSImgDirection 	Rational 	Indicates the direction of the image when it was captured. The range of values is from 0.00 to 359.99.

Exif.GPSInfo.GPSMapDatum 	Ascii 		Indicates the geodetic survey data used by the GPS receiver.
						If the survey data is restricted to Japan, the value of this tag is "TOKYO" or "WGS-84".

Exif.GPSInfo.GPSDestLatitudeRef	Ascii 		Indicates whether the latitude of the destination point is north or south latitude.
						The ASCII value "N" indicates north latitude, and "S" is south latitude.

Exif.GPSInfo.GPSDestLatitude 	Rational 	Indicates the latitude of the destination point.
						The latitude is expressed as three RATIONAL values giving the degrees, minutes, and seconds, respectively.
						If latitude is expressed as degrees, minutes and seconds, a typical format would be dd/1,mm/1,ss/1.
						When degrees and minutes are used and, for example, fractions of minutes are given up to two decimal
						places, the format would be dd/1,mmmm/100,0/1.

Exif.GPSInfo.GPSDestLongitudeRef Ascii 		Indicates whether the longitude of the destination point is east or west longitude.
						ASCII "E" indicates east longitude, and "W" is west longitude.

Exif.GPSInfo.GPSDestLongitude 	Rational 	Indicates the longitude of the destination point.
						The longitude is expressed as three RATIONAL values giving the degrees, minutes, and seconds, respectively.
						If longitude is expressed as degrees, minutes and seconds, a typical format would be ddd/1,mm/1,ss/1.
						When degrees and minutes are used and, for example, fractions of minutes are given up to two decimal
						places, the format would be ddd/1,mmmm/100,0/1.

Exif.GPSInfo.GPSDestBearingRef 	Ascii 		Indicates the reference used for giving the bearing to the destination point.
						"T" denotes true direction and "M" is magnetic direction.

Exif.GPSInfo.GPSDestBearing 	Rational 	Indicates the bearing to the destination point. The range of values is from 0.00 to 359.99.

Exif.GPSInfo.GPSDestDistanceRef Ascii 		Indicates the unit used to express the distance to the destination point. "K", "M" and "N" represent kilometers, miles and knots.

Exif.GPSInfo.GPSDestDistance 	Rational 	Indicates the distance to the destination point.

Exif.GPSInfo.GPSProcessingMethod Undefined 	A character string recording the name of the method used for location finding. The first byte indicates the character code used, and this is followed by the name of the method.

Exif.GPSInfo.GPSAreaInformation Undefined 	A character string recording the name of the GPS area.
						The first byte indicates the character code used, and this is followed by the name of the GPS area.

Exif.GPSInfo.GPSDateStamp 	Ascii 		A character string recording date and time information relative to UTC (Coordinated Universal Time).
						The format is "YYYY:MM:DD.".

Exif.GPSInfo.GPSDifferential	Short		Indicates whether differential correction is applied to the GPS receiver.
#endif
