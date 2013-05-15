#include <QString>
#include <QRectF>

#include <QDebug>

#include "MwgXmpRegion.h"

MwgXmpRegion::MwgXmpRegion()
{
	//m_stDimUnit  = MwgXmpRegion::Pixel;
	//m_stAreaUnit = MwgXmpRegion::Normalized;
	//m_focusUsage = MwgXmpRegion::NotEvaluatedNotUsed;
}

MwgXmpRegion::~MwgXmpRegion()
{
}

// Set stArea(x,y,w,h), AppliedToDimension (dimW, dimH)
// x,y,w,h will'be normalized with dimW, dimH values
void MwgXmpRegion::setRegion(int x, int y, int w, int h, int dimW, int dimH)
{
	setStDimensions(dimW, dimH);

	QRectF r(x, y, w, h);
	m_stArea.setRect(r.x()/dimW, r.y()/dimH, r.width()/dimW, r.height()/dimH);
	qDebug() << __PRETTY_FUNCTION__ << m_stArea;
}

void MwgXmpRegion::setRegion(const QRect &r, int dimW, int dimH)
{
	setRegion(r.x(), r.y(), r.width(), r.height(), dimW, dimH);
}

// Get/Set mwg-rs:Type
QString MwgXmpRegion::type() const
{
	return m_type;
}

void MwgXmpRegion::setType(const QString &type)
{
	m_type = type;
}

// Get/Set mwg-rs:Name
QString MwgXmpRegion::name() const
{
	return m_name;
}

void MwgXmpRegion::setName(const QString &name)
{
	m_name = name;
}

// Get/Set Region's description: mwg-rs:Description
QString MwgXmpRegion::description() const
{
	return m_text;
}

void MwgXmpRegion::setDescription(const QString &desc)
{
	m_text = desc;
}

// Only rectangular
enum MwgXmpRegion::Shape MwgXmpRegion::shape() const
{
	if ((m_stArea.width() > 0) && (m_stArea.height() > 0)) {
		return MwgXmpRegion::Rectangle;
	}

	if ((m_stArea.width() > 0) && (m_stArea.height() == 0)) {
		// We use m_stArea.width() as diameter [stArea:d]
		return MwgXmpRegion::Circle;
	}

	//if ((m_stArea.width() == 0) && (m_stArea.height() == 0)) {
	// correct point
	//} else {
	//  this is an invalid shape!
	//}

	return MwgXmpRegion::Point;
}

// Get/Set [mwg-rs:Area]
QRectF MwgXmpRegion::stArea() const
{
	return m_stArea;
}

void MwgXmpRegion::setStArea(qreal stAreaX, qreal stAreaY, qreal stAreaW, qreal stAreaH)
{
	m_stArea = QRectF(stAreaX, stAreaY, stAreaW, stAreaH);
	//m_stAreaUnit = MwgXmpRegion::Normalized;
}

void MwgXmpRegion::setStArea(const QRectF &stArea)
{
	m_stArea = stArea;
	//m_stAreaUnit = MwgXmpRegion::Normalized;
}

MwgXmpRegion::Unit MwgXmpRegion::stAreaUnit() const
{
	// stArea:unit is always 'normalized'
	//return m_stAreaUnit;
	return MwgXmpRegion::Normalized;
}

// Get/Set [mwg-rs:AppliedToDimensions/stDim]
QSize MwgXmpRegion::stDimensions() const
{
	return m_stDim;
}

void MwgXmpRegion::setStDimensions(int stDimW, int stDimH)
{
	m_stDim = QSize(stDimW, stDimH);
	//m_stDimUnit = MwgXmpRegion::Pixel;
}

void MwgXmpRegion::setStDimensions(const QSize &stDim)
{
	m_stDim = stDim;
	//m_stDimUnit = MwgXmpRegion::Pixel;
}

MwgXmpRegion::Unit MwgXmpRegion::stDimensionsUnit() const
{
	// stDim:unit is always 'pixel'
	//return m_stDimUnit;
	return MwgXmpRegion::Pixel;
}

#if 0
MwgXmpRegion::FocusUsage MwgXmpRegion::focusUsage() const
{
	return m_focusUsage;
}

void setFocusUsage(MwgXmpRegion::FocusUsage MwgXmpRegion::focus)
{
	m_focusUsage = focus;
}
#endif
