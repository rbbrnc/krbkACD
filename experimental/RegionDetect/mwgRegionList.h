#ifndef MWG_REGION_LIST_H
#define MWG_REGION_LIST_H

#include <QList>
#include "mwgRegion.h"

class MwgRegionList : public QList<MwgRegion>
{
	public:
		inline MwgRegionList() { }
		inline explicit MwgRegionList(const MwgRegion &r) { append(r); }
		inline MwgRegionList(const MwgRegionList &l) : QList<MwgRegion>(l) { }
		inline MwgRegionList(const QList<MwgRegion> &l) : QList<MwgRegion>(l) { }

		void detectFromFile(const QString &filePath);

	private:
};

#endif
