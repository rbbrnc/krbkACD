#include <QStringList>
#include <QDebug>

#include "XmpRegion.h"

XmpRegion::XmpRegion()
{
}

#if 0
XmpRegion::XmpRegion(enum XmpRegion::Format format, const QSize size)
	: m_format(format),
	  m_shape(Rectangle),
	  m_type(Face),
	  m_focusUsage(NotEvaluatedNotUsed),
	  m_unit(Normalized),
//	  m_name(0),
//	  m_text(0),
	  m_x(0),
	  m_y(0),
	  m_w(0),
	  m_h(0),
	  m_d(0),
//	  m_emailDigest(0),
//	  m_liveCID(0),
//	  m_barcodeValue(0),
	  m_appliedToDimensionsUnit(XmpRegion::Pixel)
{
	m_appliedToDimensionsWidth  = size.width();
	m_appliedToDimensionsHeight = size.height();
}
#endif

XmpRegion::XmpRegion(const XmpRegion& other)
	: m_format(other.m_format),
	  m_shape(other.m_shape),
	  m_type(other.m_type),
	  m_unit(other.m_unit),
	  m_focusUsage(other.m_focusUsage),
	  m_name(other.m_name),
	  m_text(other.m_text),
	  m_boundingRect(other.m_boundingRect),
	  m_x(other.m_x),
	  m_y(other.m_y),
	  m_w(other.m_w),
	  m_h(other.m_h),
	  m_d(other.m_d),
	  m_emailDigest(other.m_emailDigest),
	  m_liveCID(other.m_liveCID),
	  m_barcodeValue(other.m_barcodeValue),
	  m_dimW(other.m_dimW),
	  m_dimH(other.m_dimH),
	  m_dimUnit(other.m_dimUnit)
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

	m_x = other.m_x;
	m_y = other.m_y;
	m_w = other.m_w;
	m_h = other.m_h;
	m_d = other.m_d;

	m_emailDigest = other.m_emailDigest;
	m_liveCID     = other.m_liveCID;

	m_barcodeValue = other.m_barcodeValue;
	m_focusUsage   = other.m_focusUsage;

	m_dimW = other.m_dimW;
	m_dimH = other.m_dimH;
	m_dimUnit = other.m_dimUnit;

	return *this;
}

void XmpRegion::setFormat(enum XmpRegion::Format format)
{
	m_format = format;
}


void XmpRegion::setFocusUsage(XmpRegion::FocusUsage focus)
{
	m_focusUsage = focus;
}

void XmpRegion::setType(XmpRegion::Type type)
{
	m_type = type;
}

enum XmpRegion::Type XmpRegion::type() const
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

QRectF XmpRegion::boundingRect() const
{
/*
*/
	return m_boundingRect;
}

void XmpRegion::setSize(qreal x, qreal y, qreal w, qreal h, qreal d, qreal dimW, qreal dimH)
{
	m_x = x;
	m_y = y;
	m_w = w;
	m_h = h;
	m_d = d;

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
bool XmpRegion::setRegion(const QRectF &regionRect, const QSize &imageSize)
{
	if (imageSize.isValid()) {
		m_boundingRect = regionRect;

		// Normalized Values
		m_x = regionRect.x() / imageSize.width();
		m_y = regionRect.y() / imageSize.height();
		m_w = regionRect.width() / imageSize.width();
		m_h = regionRect.height() / imageSize.height();
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
	         << "P(x,y)" << QPointF(m_x, m_y)
		 << "S(w,h)" << QSizeF(m_w, m_h)
		 << "D:" << m_d
		 << "Bounding Rect:" << m_boundingRect
		 << "Shape:" << shape << "[" << m_shape << "]"
		 << "Name:" << m_name
		 << "Description:" << m_text;

//		 << "Region Type:" << m_type
//		 << "m_region:" << m_region;
}
