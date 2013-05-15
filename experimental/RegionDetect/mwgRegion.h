#ifndef MWG_REGION_H
#define MWG_REGION_H

#include <QString>
#include <QRect>
#include <QRectF>
#include <QMetaType>
#include <QDebug>

class MwgRegion
{
	public:
		enum Type  { Face = 0, Pet, Focus, Barcode };
		enum Shape { Rectangle = 0, Circle, Point };
		enum Unit  { Normalized = 0, Pixel };

		enum FocusUsage {
			EvaluatedNotUsed = 0, // Evaluated, Not Used
			EvaluatedUsed,        // Evaluated, Used
			NotEvaluatedNotUsed   // Not Evaluated, Not Used
		};

		MwgRegion(const QRect &area = QRect(0, 0, 0, 0),
			  const QSize &dim = QSize(0, 0),
			   MwgRegion::Type type = MwgRegion::Focus);

		~MwgRegion();

		// Set stArea(x,y,w,h), AppliedToDimension (dimW, dimH)
		// x,y,w,h will'be normalized with dimW, dimH values
		void setRegion(int x, int y, int w, int h, int dimW, int dimH);
		void setRegion(const QRect &r, int dimW, int dimH);
		void setRegion(const QRect &r, const QSize &dim);

		// Get/Set mwg-rs:Type
		MwgRegion::Type type() const;
		void setType(MwgRegion::Type type);

		// Get/Set mwg-rs:Name
		QString name() const;
		void setName(const QString &name);

		// Get/Set Region's description: mwg-rs:Description
		QString description() const;
		void setDescription(const QString &desc);

		// Region's shape
		enum MwgRegion::Shape shape() const;

		// Get/Set [mwg-rs:Area]
		QRectF stArea() const;
		void setStArea(qreal stAreaX, qreal stAreaY, qreal stAreaW, qreal stAreaH);
		void setStArea(const QRectF &stArea);
		MwgRegion::Unit stAreaUnit() const;

		// Get/Set [mwg-rs:AppliedToDimensions/stDim]
		QSize stDimensions() const;
		void setStDimensions(int stDimW, int stDimH);
		void setStDimensions(const QSize &stDim);
		MwgRegion::Unit stDimensionsUnit() const;

		MwgRegion::FocusUsage focusUsage() const;
		void setFocusUsage(MwgRegion::FocusUsage focus);

	private:
		// Region type: mwg-rs:Type = [Face | Pet | Focus | Barcode]
		MwgRegion::Type m_type;

		// Region's name: mwg-rs:Name
		QString	m_name;

		// Region's description: mwg-rs:Description
		QString	m_text;

		// Image dimensions [mwg-rs:AppliedToDimensions/]
		// stDim:w, stDim:h, stDim:unit
		QSize  m_stDim;
		//enum MwgRegion::Unit m_stDimUnit;

		// Normalized Region rectangle [mwg-rs:Area]
		// stArea:x, stArea:y, stArea:w, stArea:h, stArea:unit
		QRectF m_stArea;
		//enum MwgRegion::Unit m_stAreaUnit;

		enum MwgRegion::FocusUsage m_focusUsage;
};

// Declare MwgRegion as a custom Meta Type
Q_DECLARE_METATYPE(MwgRegion);

// MwgRegion streaming operator
QDebug operator << (QDebug dbg, const MwgRegion &r);

#endif
