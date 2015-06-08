#ifndef MWG_REGION_H
#define MWG_REGION_H

#include <QString>
#include <QRect>
#include <QRectF>
#include <QMetaType>
#include <QDebug>

#include <QJsonArray>
#include <QJsonObject>

// MWG Region Schema enums
namespace MwgRs {
	enum Type  { Face = 0, Pet, Focus, Barcode };
	enum Shape { Rectangle = 0, Circle, Point };
	enum Unit  { Normalized = 0, Pixel };

	enum FocusUsage {
		EvaluatedNotUsed = 0, // Evaluated, Not Used
		EvaluatedUsed,        // Evaluated, Used
		NotEvaluatedNotUsed   // Not Evaluated, Not Used
	};

	const QString regionListTag(const QString &tagName, int index);
}

class MwgRegion
{
	public:
		MwgRegion(const QRectF &area = QRectF(0, 0, 0, 0),
			  const QSizeF &dim = QSizeF(0, 0),
			  bool normalized = true);

		~MwgRegion();

		// Set stArea(x,y,w,h), AppliedToDimension (dimW, dimH)
		// x,y,w,h will'be normalized with dimW, dimH values
		void setRegion(qreal x, qreal y, qreal w, qreal h, qreal dimW, qreal dimH, bool normalized);
		void setRegion(const QRectF &area, const QSizeF &dim, bool normalized);

		// Get/Set mwg-rs:Type ad focus usage
		MwgRs::Type type() const;
		MwgRs::FocusUsage focusUsage() const;

        void setType(MwgRs::Type type, MwgRs::FocusUsage focus = MwgRs::NotEvaluatedNotUsed);

		// Get/Set mwg-rs:Name
		QString name() const;
		void setName(const QString &name);

		// Get/Set Region's description: mwg-rs:Description
		QString description() const;
		void setDescription(const QString &desc);

		// Region's shape
		MwgRs::Shape shape() const;

		// Get/Set [mwg-rs:Area]
		QRectF stArea() const;
		MwgRs::Unit stAreaUnit() const;

		// Get [mwg-rs:AppliedToDimensions/stDim]
		QSizeF stDimensions() const;
		MwgRs::Unit stDimensionsUnit() const;

		// return the stArea bounding box
		QRectF stAreaBoundingRectF() const;

        QJsonObject toJson() const;

	private:
		// Region type: mwg-rs:Type
		MwgRs::Type m_type;

		// Region's name: mwg-rs:Name
		QString	m_name;

		// Region's description: mwg-rs:Description
		QString	m_text;

		// Image dimensions [mwg-rs:AppliedToDimensions/]
		// stDim:w, stDim:h, stDim:unit
		QSizeF m_stDim;
		MwgRs::Unit m_stDimUnit;

		// Normalized Region rectangle [mwg-rs:Area]
		// stArea:x, stArea:y, stArea:w, stArea:h, stArea:unit
		QRectF m_stArea;
		MwgRs::Unit m_stAreaUnit;

		// Rectangular bounding box for stArea shape
		// used for display
		QRectF m_stAreaBoundingRect;

		MwgRs::FocusUsage m_focusUsage;

		MwgRs::Shape m_shape;
};

// Declare MwgRegion as a custom Meta Type
Q_DECLARE_METATYPE(MwgRegion);

#ifndef QT_NO_DEBUG_OUTPUT
// MwgRegion streaming operator
QDebug operator << (QDebug dbg, const MwgRegion &r);
#endif


class MwgRegionList : public QList<MwgRegion>
{
	public:
		inline MwgRegionList() { }
		inline explicit MwgRegionList(const MwgRegion &r) { append(r); }
		inline MwgRegionList(const MwgRegionList &l) : QList<MwgRegion>(l) { }
		inline MwgRegionList(const QList<MwgRegion> &l) : QList<MwgRegion>(l) { }

        QJsonArray toJson() const;
};

#endif
