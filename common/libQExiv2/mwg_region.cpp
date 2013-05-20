#include <QString>
#include <QRectF>
#include <QDebug>

#include "mwg_region.h"

MwgRegion::MwgRegion(const QRect &area, const QSize &dim, MwgRegion::Type type)
	: m_type(type),
	  m_stDimUnit(MwgRegion::Pixel),
	  m_stAreaUnit(MwgRegion::Normalized),
	  m_focusUsage(MwgRegion::NotEvaluatedNotUsed)
{
	// Normalize area
	if ((dim.width() > 0) && (dim.height() > 0)) {
		setRegion(area, dim);
	}
}

MwgRegion::~MwgRegion()
{
}

// Set stArea(x,y,w,h), AppliedToDimension (dimW, dimH)
// x,y,w,h will'be normalized with dimW, dimH values
void MwgRegion::setRegion(int x, int y, int w, int h, int dimW, int dimH)
{
	setStDimensions(dimW, dimH);

	QRectF r(x, y, w, h);
	m_stArea.setRect(r.x()/dimW, r.y()/dimH, r.width()/dimW, r.height()/dimH);
	qDebug() << __PRETTY_FUNCTION__ << m_stArea;
}


void MwgRegion::setRegion(const QRect &r, const QSize &dim)
{
	setRegion(r.x(), r.y(), r.width(), r.height(), dim.width(), dim.height());
}

void MwgRegion::setRegion(const QRect &r, int dimW, int dimH)
{
	setRegion(r.x(), r.y(), r.width(), r.height(), dimW, dimH);
}

// Get/Set mwg-rs:Type
MwgRegion::Type MwgRegion::type() const
{
	return m_type;
}

void MwgRegion::setType(MwgRegion::Type type)
{
	m_type = type;
}

// Get/Set mwg-rs:Name
QString MwgRegion::name() const
{
	return m_name;
}

void MwgRegion::setName(const QString &name)
{
	m_name = name;
}

// Get/Set Region's description: mwg-rs:Description
QString MwgRegion::description() const
{
	return m_text;
}

void MwgRegion::setDescription(const QString &desc)
{
	m_text = desc;
}

// Only rectangular
enum MwgRegion::Shape MwgRegion::shape() const
{
	if ((m_stArea.width() > 0) && (m_stArea.height() > 0)) {
		return MwgRegion::Rectangle;
	}

	if ((m_stArea.width() > 0) && (m_stArea.height() == 0)) {
		// We use m_stArea.width() as diameter [stArea:d]
		return MwgRegion::Circle;
	}

	//if ((m_stArea.width() == 0) && (m_stArea.height() == 0)) {
	// correct point
	//} else {
	//  this is an invalid shape!
	//}

	return MwgRegion::Point;
}

// Get/Set [mwg-rs:Area]
QRectF MwgRegion::stArea() const
{
	return m_stArea;
}

void MwgRegion::setStArea(qreal stAreaX, qreal stAreaY,
			  qreal stAreaW, qreal stAreaH,
			  enum MwgRegion::Unit unit)
{
	setStArea(QRectF(stAreaX, stAreaY, stAreaW, stAreaH), unit);
}

void MwgRegion::setStArea(const QRectF &stArea, enum MwgRegion::Unit unit)
{
	m_stArea = stArea;
	m_stAreaUnit = unit;
}

MwgRegion::Unit MwgRegion::stAreaUnit() const
{
	return m_stAreaUnit;
}

// Get/Set [mwg-rs:AppliedToDimensions/stDim]
QSize MwgRegion::stDimensions() const
{
	return m_stDim;
}

void MwgRegion::setStDimensions(int stDimW, int stDimH, enum MwgRegion::Unit unit)
{
	setStDimensions(QSize(stDimW, stDimH), unit);
}

void MwgRegion::setStDimensions(const QSize &stDim, enum MwgRegion::Unit unit)
{
	m_stDim = stDim;
	m_stDimUnit = unit;
}

MwgRegion::Unit MwgRegion::stDimensionsUnit() const
{
	return m_stDimUnit;
}

MwgRegion::FocusUsage MwgRegion::focusUsage() const
{
	return m_focusUsage;
}

void MwgRegion::setFocusUsage(MwgRegion::FocusUsage focus)
{
	m_focusUsage = focus;
}

// Check compile with NO_DEBUG
QDebug operator << (QDebug dbg, const MwgRegion &r)
{
	QRectF stArea = r.stArea();
	QSize  stDim  = r.stDimensions();

	QString s;
	s = "Xmp.mwg-rs.Regions ";
	s += "\n  mwg-rs:AppliedToDimensions ";
	s += "\n   stDim:w " + stDim.width();
	s += "\n   stDim:h " + stDim.height();
	s += "\n   stDim:unit pixel";
	s += "\n  mwg-rs:Name " + r.name();
	s += "\n  mwg-rs:Description " + r.description();
	s += "\n  mwg-rs:Type ";
	switch (r.type()) {
	case MwgRegion::Face:
		s += "Face";
		break;
	case MwgRegion::Pet:
		s += "Pet";
		break;
	case MwgRegion::Focus:
		s += "Focus";
		s += "\n  mwg-rs:FocusUsage " + QString::number(r.focusUsage());
		break;
	case MwgRegion::Barcode:
		s += "Barcode";
		s += "\n  mwg-rs:BarCodeValue Not implemented";
		break;
	default:
		s += "???";
		break;
	}

	s += "\n  mwg-rs:Area";
	s += "\n    stArea:x " + QString::number(stArea.x());
	s += "\n    stArea:y " + QString::number(stArea.y());
	s += "\n    stArea:w " + QString::number(stArea.width());
	s += "\n    stArea:h " + QString::number(stArea.height());
//	s += "\n    stArea:d " + QString::number();
	s += "\n    stArea:unit normalized";

        dbg.nospace() << "MwgRegion(" << s << ")";
        return dbg.maybeSpace();
}

