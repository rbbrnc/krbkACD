#include <QString>
#include <QRectF>
#include <QDebug>

#include "mwg_region.h"

// Returns a RegionList tag at 'index'
const QString MwgRs::regionListTag(const QString &tagName, int index)
{
	if (index <= 0) {
		return QString();
	}

	QString s = QString("Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]").arg(index);
	if (!tagName.isEmpty()) {
		s += "/mwg-rs:" + tagName;
	}
	return s;
}

MwgRegion::MwgRegion(const QRectF &area, const QSizeF &dim, bool normalized)
	: m_type(MwgRs::Focus),
	  m_stDimUnit(MwgRs::Pixel),
	  m_stAreaUnit(MwgRs::Normalized),
	  m_stAreaBoundingRect(QRectF()),
	  m_focusUsage(MwgRs::NotEvaluatedNotUsed),
	  m_shape(MwgRs::Point)
{
	// Normalize area
	if ((dim.width() > 0) && (dim.height() > 0)) {
		setRegion(area, dim, normalized);
	}
}

MwgRegion::~MwgRegion()
{
}

// Set stArea(x,y,w,h), AppliedToDimension (dimW, dimH)
void MwgRegion::setRegion(qreal x, qreal y, qreal w, qreal h, qreal dimW, qreal dimH, bool normalized)
{
	if ((dimW > 0) && (dimH > 0)) {
		m_stDim = QSizeF(dimW, dimH);

		if (normalized) {
			m_stArea.setRect(x, y, w, h);
		} else {
			// x,y,w,h will'be normalized with dimW, dimH values
			m_stArea.setRect(x/dimW, y/dimH, w/dimW, h/dimH);
		}

		//qDebug() << __PRETTY_FUNCTION__ << m_stArea;
	} else {
		qWarning() << __PRETTY_FUNCTION__
			   << "Invalid stDim:" << QSizeF(dimW, dimH)
			   << "stArea:" << QRectF(x, y, w, h);
		return;
	}

	if ((w > 0) && (h > 0)) {
		m_shape = MwgRs::Rectangle;
		if (normalized) {
			m_stAreaBoundingRect = QRectF(x*dimW, y*dimH, w*dimW, h*dimH);
		} else {
			m_stAreaBoundingRect = QRectF(x, y, w, h);
		}
	} else if  ((w > 0) && (h <= 0)) {
		// We use m_stArea.width() as diameter [stArea:d]
		m_shape = MwgRs::Circle;
		if (normalized) {
			m_stAreaBoundingRect = QRectF(x*dimW, y*dimH/2, w*dimW, w*dimW/2);
		} else {
			m_stAreaBoundingRect = QRectF(x, y, w/2, w/2);
		}

	} else {
		//if ((w <= 0) && (h <= 0)) {
		m_shape = MwgRs::Point;
		// bounding rect is 2 pixel wide
		if (normalized) {
			m_stAreaBoundingRect = QRectF(x*dimW, y*dimH, 2, 2);
		} else {
			m_stAreaBoundingRect = QRectF(x, y, 2, 2);
		}
	}

}
void MwgRegion::setRegion(const QRectF &area, const QSizeF &dim, bool normalized)
{
	setRegion(area.x(), area.y(), area.width(), area.height(),
		  dim.width(), dim.height(), normalized);
}

// Get/Set mwg-rs:Type
MwgRs::Type MwgRegion::type() const
{
	return m_type;
}

void MwgRegion::setType(MwgRs::Type type, MwgRs::FocusUsage focus)
{
	m_type = type;
	if (type == MwgRs::Focus) {
		m_focusUsage = focus;
	} else {
		m_focusUsage = MwgRs::NotEvaluatedNotUsed;
	}
}

MwgRs::FocusUsage MwgRegion::focusUsage() const
{
	return m_focusUsage;
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
MwgRs::Shape MwgRegion::shape() const
{
	return m_shape;
}

QRectF MwgRegion::stAreaBoundingRectF() const
{
	return m_stAreaBoundingRect;
}

// Get [mwg-rs:Area]
QRectF MwgRegion::stArea() const
{
	return m_stArea;
}

MwgRs::Unit MwgRegion::stAreaUnit() const
{
	return m_stAreaUnit;
}

// Get/Set [mwg-rs:AppliedToDimensions/stDim]
QSizeF MwgRegion::stDimensions() const
{
	return m_stDim;
}

MwgRs::Unit MwgRegion::stDimensionsUnit() const
{
	return m_stDimUnit;
}

// Check compile with NO_DEBUG
QDebug operator << (QDebug dbg, const MwgRegion &r)
{
	QRectF stArea = r.stArea();

	QString s;
	s = "Xmp.mwg-rs.Regions ";
	s += "\n  mwg-rs:AppliedToDimensions ";
	s += "\n   stDim:w " + QString::number(r.stDimensions().width());
	s += "\n   stDim:h " + QString::number(r.stDimensions().height());
	s += "\n   stDim:unit pixel";
	s += "\n  mwg-rs:Name " + r.name();
	s += "\n  mwg-rs:Description " + r.description();
	s += "\n  mwg-rs:Type ";
	switch (r.type()) {
	case MwgRs::Face:
		s += "Face";
		break;
	case MwgRs::Pet:
		s += "Pet";
		break;
	case MwgRs::Focus:
		s += "Focus";
		s += "\n  mwg-rs:FocusUsage " + QString::number(r.focusUsage());
		break;
	case MwgRs::Barcode:
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

