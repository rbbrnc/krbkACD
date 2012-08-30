#ifndef MWG_REGION_H
#define MWG_REGION_H

#include <QString>
#include <QRectF>
#include <QPointF>

class MwgArea
{
	public:
		MwgArea();
		MwgArea(const MwgArea &other);
		~MwgArea();

		MwgArea& operator=(const MwgArea &other);

		enum Type {
			Rectangle = 0,
			Circle,
			Point
		};

		enum MwgArea::Type type() const;

		QRectF rect() const;
		QPointF point() const;

	private:
		enum Type m_type;

		double m_x;	// X coordinate of the center of the area (point, circle, rectangle)
		double m_y;	// Y coordinate of the center of the area (point, circle, rectangle)
		double m_w;	// Width of the area (rectangle)
		double m_h;	// Height of the area (rectangle)
		double m_d;	// Diameter of area (circle)
		QString m_unit; // Only "normalized"
};

class MwgRegionInfo
{
	public:
		MwgRegionInfo() {}

		QString name() const;
		void setName(const QString &name);

		QString description() const;
		void setDescription(const QString &description);

		QString type() const;
		void setType(const QString &type);

		QString barcodeValue() const;
		void setBarcodeValue(const QString &value);

		MwgArea area() const;
		void setArea(const MwgArea &area);

	private:
		QString m_name;
		QString m_description;
		QString m_type;		// "Face" | "Pet" | "Focus" | "BarCode"
		QString m_barcodeValue;

		MwgArea m_area;
};

#endif
