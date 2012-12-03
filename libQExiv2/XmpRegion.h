#ifndef XMP_REGION_H
#define XMP_REGION_H

#include <QString>
#include <QRectF>

class XmpRegion
{
	public:
		enum Format {
			MWG = 0,
			MP
		};

		enum Shape {
			Rectangle = 0,
			Circle,
			Point
		};

		enum Type {
			Face = 0,
			Pet,
			Focus,
			BarCode
		};

		enum Unit {
			Normalized = 0,
			Pixel
		};

		enum FocusUsage {
			EvaluatedNotUsed = 0, // Evaluated, Not Used
			EvaluatedUsed,        // Evaluated, Used
			NotEvaluatedNotUsed   // Not Evaluated, Not Used
		};

		XmpRegion();
		//XmpRegion(enum XmpRegion::Format format, const QSize size);
		XmpRegion(const XmpRegion &other);
		~XmpRegion();

		XmpRegion& operator=(const XmpRegion &other);

		// Set
		void setFormat(enum XmpRegion::Format format);
		void setType(XmpRegion::Type type);
		void setFocusUsage(XmpRegion::FocusUsage focus);

		// set point(x,y), size(w,h), diameter, AppliedToDimension (dimW, dimH)
		void setSize(qreal x, qreal y, qreal w, qreal h, qreal d, qreal dimW, qreal dimH);

		void setName(const QString &name);
		void setDescription(const QString &desc);

		// Get
		enum XmpRegion::Format format() const;
		enum XmpRegion::Shape  shape() const;
		enum XmpRegion::Type   type() const;

		QString name() const;
		QString description() const;

		QRectF boundingRect() const;

		bool setRegion(const QRectF &regionRect, const QSize &imageSize);

		void setRectRegion(const QString &region);
		void setRectRegion(const QRectF &rect);


		void debug() const;

	private:
		enum XmpRegion::Format m_format; // MP or MWG
		enum XmpRegion::Shape  m_shape;  // MWG Region's shape
		enum XmpRegion::Type   m_type;   // MWG region type
		enum XmpRegion::Unit   m_unit;   // MWG Only "normalized"

		enum XmpRegion::FocusUsage m_focusUsage; // MWG Focus usage

		QString	m_name;   // Region's name
		QString	m_text;	  // Region's description

		QRectF m_boundingRect; // Region rect

		// Normalized [0, 1] values
		qreal m_x;  // X coordinate of the center of the area (point, circle, rectangle)
		qreal m_y;  // Y coordinate of the center of the area (point, circle, rectangle)
		qreal m_w;  // Width of the area (rectangle)
		qreal m_h;  // Height of the area (rectangle)
		qreal m_d;  // Diameter of area (circle)

		// MP
		// http://msdn.microsoft.com/en-us/library/ee719905%28VS.85%29.aspx
		//
		QString m_emailDigest;	// PersonEmailDigest
		QString m_liveCID;	// PersonLiveCID

		// MWG
		QString m_barcodeValue;

		// Applied To Dimensions
		int m_dimW; // mwg-rs:AppliedToDimensions/stDim:w
		int m_dimH; // mwg-rs:AppliedToDimensions/stDim:h
		enum XmpRegion::Unit m_dimUnit; // MWG Only "pixel"
};

#endif
