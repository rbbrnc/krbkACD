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
		PTag();
		PTag(const PTag &other);
		~PTag();

		PTag& operator=(const PTag &other);


		QString text() const;
		void setText(const QString &text);

		QRectF region() const;
		void setRegion(const QString &region);

		void debug() const;

	private:
		QSharedDataPointer<PTagDataPrivate> d;
};

#endif
