#ifndef MWG_REGION_INFO_H
#define MWG_REGION_INFO_H

#include <QString>
#include <QList>

struct mwgRegionArea {
	double x; // pos x (top left corner or center
	double y; // pos y (top left corner or center)
	double w; // width
	double h; // height;
	double d; // diameter;
	QString unit;
};

struct mwgDimensions {
	double height;
	double width;
	QString unit;
};

struct mwgRegion {
	struct mwgRegionArea area;
	QString name;
	QString description;
	QString type;		// 'BarCode' | 'Face' | 'Focus' | 'Pet'
	QString barCodeValue;
	QString focusUsage;	// 'EvaluatedNotUsed'    = Evaluated, Not Used
				// 'EvaluatedUsed'       = Evaluated, Used
				// 'NotEvaluatedNotUsed' = Not Evaluated, Not Used

//	struct mwgExtensions extensions;
};

/*
struct mwgRegionInfo {
	struct mwgDimensions appliedToDimensions;
	QList<mwgRegion> regionList;
};
*/

class MwgRegionInfo
{
	public:
		MwgRegionInfo();

		void setDimensions();
		void insertRegion(QSizeF atd, QRectF sel, const QString &name);
		void removeRegion();
		void save();

	private:
		//struct mwgRegionInfo m_regionInfo;
		struct mwgDimensions m_appliedToDimensions;
		QList<mwgRegion>     m_regionList;

};

#endif
