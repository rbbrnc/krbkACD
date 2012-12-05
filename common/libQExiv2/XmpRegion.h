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
		XmpRegion(const XmpRegion &other);
		~XmpRegion();

		XmpRegion& operator=(const XmpRegion &other);

		// Set
		enum XmpRegion::Format format() const;
		void setFormat(enum XmpRegion::Format format);

		void setFocusUsage(XmpRegion::FocusUsage focus);

		// set point(x,y), size(w,h), diameter, AppliedToDimension (dimW, dimH)
		void setSize(qreal x, qreal y, qreal w, qreal h, qreal d, qreal dimW, qreal dimH);
		bool setRegion(const QRectF &regionRect, const QSize &imageSize);

		QString type() const;
		void setType(const QString &type);

		QString name() const;
		void setName(const QString &name);

		QString description() const;
		void setDescription(const QString &desc);

		QString personEmailDigest() const;
		void setPersonEmailDigest(const QString &text);

		QString personLiveCID() const;
		void setPersonLiveCID(const QString &text);

		void setData(void *data);
		void *data() const;

		enum XmpRegion::Shape  shape() const;

		QRectF boundingRect() const;

		void debug() const;

	private:
		enum XmpRegion::Format m_format; // MP or MWG
		enum XmpRegion::Shape  m_shape;  // MWG Region's shape
		enum XmpRegion::Unit   m_unit;   // MWG Only "normalized"

		enum XmpRegion::FocusUsage m_focusUsage; // MWG Focus usage

		QString m_type;   // MWG region type (Face, Pet, Focus, Barcode)
		QString	m_name;   // Region's name
		QString	m_text;	  // Region's description

		QRectF m_boundingRect; // Region rect

		// Normalized [0, 1] values
		qreal m_stAreaX;  // X coordinate of the center of the area (point, circle, rectangle)
		qreal m_stAreaY;  // Y coordinate of the center of the area (point, circle, rectangle)
		qreal m_stAreaW;  // Width of the area (rectangle)
		qreal m_stAreaH;  // Height of the area (rectangle)
		qreal m_stAreaD;  // Diameter of area (circle)

		// MP
		// http://msdn.microsoft.com/en-us/library/ee719905%28VS.85%29.aspx
		//
		QString m_personEmailDigest;  // PersonEmailDigest
		QString m_personLiveCID;      // PersonLiveCID

		// MWG
		QString m_barcodeValue;

		// Applied To Dimensions
		int m_dimW; // mwg-rs:AppliedToDimensions/stDim:w
		int m_dimH; // mwg-rs:AppliedToDimensions/stDim:h
		enum XmpRegion::Unit m_dimUnit; // MWG Only "pixel"

		// User's data
		void *m_data;
};

#endif
