#ifndef MP_REGION_H
#define MP_REGION_H

#include <QString>
#include <QRectF>
#include <QPointF>

// http://msdn.microsoft.com/en-us/library/ee719905%28VS.85%29.aspx
//
class MPRegionInfo
{
	public:
		MPRegionInfo() {}

		QString name() const;
		void setName(const QString &name);

		QRectF rect() const;
		void setRect(const QString &rect);

	private:
		QString m_name;		// PersonDisplayName
		QRectF	m_rect;		// MPReg:Rectangle (Normalized [0, 1])
		QString m_emailDigest;	// PersonEmailDigest
		QString m_liveCID;	// PersonLiveCID
};

#endif
