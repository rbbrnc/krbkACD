#ifndef P_TAG_H
#define P_TAG_H

#include <QSharedDataPointer>
#include <QString>
#include <QRectF>

// Forward decl
class PTagDataPrivate;

class PTag
{
	public:
		enum RegionType {
			MwgRegionType = 0,
			MpRegionType
		};

		PTag();
		PTag(const PTag &other);
		~PTag();

		PTag& operator=(const PTag &other);

		QString description() const;
		void setDescription(const QString &desc);

		QRectF region() const;
		void setRegion(const QString &region);

		enum PTag::RegionType type() const;
		void setType(PTag::RegionType type);

		void debug() const;

		QString name() const;
		void setName(const QString &name);

	private:
		QSharedDataPointer<PTagDataPrivate> d;

		enum PTag::RegionType m_type;
};

#endif
