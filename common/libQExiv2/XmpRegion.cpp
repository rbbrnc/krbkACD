#include <QStringList>
#include <QDebug>

#include "XmpRegion.h"

XmpRegion::XmpRegion()
{
	m_stAreaUnit = "normalized";
	m_dimUnit    = "pixel";
}

XmpRegion::XmpRegion(const XmpRegion& other)
	: m_format(other.m_format),
	  m_shape(other.m_shape),
	  m_unit(other.m_unit),
	  m_focusUsage(other.m_focusUsage),
	  m_type(other.m_type),
	  m_name(other.m_name),
	  m_text(other.m_text),
	  m_boundingRect(other.m_boundingRect),
	  m_stAreaX(other.m_stAreaX),
	  m_stAreaY(other.m_stAreaY),
	  m_stAreaW(other.m_stAreaW),
	  m_stAreaH(other.m_stAreaH),
	  m_stAreaD(other.m_stAreaD),
	  m_stAreaUnit(other.m_stAreaUnit),
	  m_personEmailDigest(other.m_personEmailDigest),
	  m_personLiveCID(other.m_personLiveCID),
	  m_barcodeValue(other.m_barcodeValue),
	  m_dimW(other.m_dimW),
	  m_dimH(other.m_dimH),
	  m_dimUnit(other.m_dimUnit),
	  m_data(0)
{
}

XmpRegion::~XmpRegion()
{
}

XmpRegion& XmpRegion::operator=(const XmpRegion& other)
{
	m_format = other.m_format;
	m_shape  = other.m_shape;
	m_type   = other.m_type;
	m_unit   = other.m_unit;
	m_name   = other.m_name;
	m_text   = other.m_text;

	m_boundingRect = other.m_boundingRect;

	// set stArea
	m_stAreaX    = other.m_stAreaX;
	m_stAreaY    = other.m_stAreaY;
	m_stAreaW    = other.m_stAreaW;
	m_stAreaH    = other.m_stAreaH;
	m_stAreaD    = other.m_stAreaD;
	m_stAreaUnit = other.m_stAreaUnit;

	m_personEmailDigest = other.m_personEmailDigest;
	m_personLiveCID     = other.m_personLiveCID;

	m_barcodeValue = other.m_barcodeValue;
	m_focusUsage   = other.m_focusUsage;

	m_dimW = other.m_dimW;
	m_dimH = other.m_dimH;
	m_dimUnit = other.m_dimUnit;

	m_data = other.m_data;

	return *this;
}

enum XmpRegion::Shape XmpRegion::shape() const
{
	return m_shape;
}

qreal XmpRegion::stAreaX() const
{
	return m_stAreaX;
}
qreal XmpRegion::stAreaY() const
{
	return m_stAreaY;
}
qreal XmpRegion::stAreaW() const
{
	return m_stAreaW;
}
qreal XmpRegion::stAreaH() const
{
	return m_stAreaH;
}

qreal XmpRegion::stAreaD() const
{
	return m_stAreaD;
}

void XmpRegion::setFormat(enum XmpRegion::Format format)
{
	m_format = format;
}

void XmpRegion::setFocusUsage(XmpRegion::FocusUsage focus)
{
	m_focusUsage = focus;
}

void XmpRegion::setType(const QString &type)
{
	m_type = type;
}

QString XmpRegion::type() const
{
	return m_type;
}

QString XmpRegion::description() const
{
	return m_text;
}

void XmpRegion::setDescription(const QString &desc)
{
	m_text = desc;
}

QString XmpRegion::name() const
{
	return m_name;
}

void XmpRegion::setName(const QString &name)
{
	m_name = name;
}

QString XmpRegion::areaUnit() const
{
	return m_stAreaUnit;
}

void XmpRegion::setAreaUnit(const QString &unit)
{
	m_stAreaUnit = unit;
}

QString XmpRegion::dimensionUnit() const
{
	return m_dimUnit;
}

void XmpRegion::setDimensionUnit(const QString &unit)
{
	m_dimUnit = unit;
}

QString XmpRegion::personEmailDigest() const
{
	return m_personEmailDigest;
}

QString XmpRegion::personLiveCID() const
{
	return m_personLiveCID;
}

void XmpRegion::setPersonEmailDigest(const QString &text)
{
	m_personEmailDigest = text;
}

void XmpRegion::setPersonLiveCID(const QString &text)
{
	m_personLiveCID = text;
}

QRectF XmpRegion::boundingRect() const
{
	return m_boundingRect;
}

void XmpRegion::setSize(qreal x, qreal y, qreal w, qreal h, qreal d, qreal dimW, qreal dimH)
{
	m_stAreaX = x;
	m_stAreaY = y;
	m_stAreaW = w;
	m_stAreaH = h;
	m_stAreaD = d;

	// Set Shape
	if (d > 0) {
		m_shape = XmpRegion::Circle;
		m_boundingRect = QRectF(x * dimW, y * dimH, (d * dimW)/2, (d * dimH)/2);
	} else if ((w > 0)  && (h > 0)) {
		m_shape = XmpRegion::Rectangle;
		m_boundingRect = QRectF(x * dimW, y * dimH, w * dimW, h *dimH);
	} else {
		m_shape = XmpRegion::Point;
		// bounding rect is 2 pixel wide
		m_boundingRect = QRectF(x * dimW, y * dimH, 2, 2);
	}
}

// Rectangular regions only!
// stArea:x = x / dimW
// stArea:y = y / dimH
//
// x = stArea:x * dimW
// y = stArea:y * dimH
//
bool XmpRegion::setRegion(const QRectF &regionRect, const QSize &imageSize)
{
	if (imageSize.isValid()) {
		m_boundingRect = regionRect;

		// Normalized Values
		m_stAreaX = regionRect.x() / imageSize.width();
		m_stAreaY = regionRect.y() / imageSize.height();
		m_stAreaW = regionRect.width() / imageSize.width();
		m_stAreaH = regionRect.height() / imageSize.height();
		return true;
	}

	qDebug() << __PRETTY_FUNCTION__ << "Invalid imageSize:" << imageSize;
	return false;
}

void XmpRegion::debug() const
{
	QString shape;
	switch (m_shape) {
	case XmpRegion::Circle:
		shape = "Circle";
		break;
	case XmpRegion::Rectangle:
		shape = "Rectangle";
		break;
	case XmpRegion::Point:
		shape = "Point";
		break;
	default:
		break;
	}

	qDebug() << __PRETTY_FUNCTION__
	         << "P(x,y,w,h,d)"
		 << m_stAreaX
		 << m_stAreaY
		 << m_stAreaW
		 << m_stAreaH
		 << m_stAreaD
		 << "Bounding Rect:" << m_boundingRect
		 << "Shape:" << shape
		 << "Name:" << m_name
		 << "Description:" << m_text
		 << "Type:" << m_type;
//		 << "m_region:" << m_region;
}

void XmpRegion::setData(void *data)
{
	m_data = data;
}

void *XmpRegion::data() const
{
	return m_data;
}
