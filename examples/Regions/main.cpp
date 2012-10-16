#include <QtGui>

#include "QExiv2.h"
#include "RegionEditManager.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	RegionEditManager manager;

	QString fileName;
	if (argc >= 2) {
		fileName = argv[1];
		manager.setFile(fileName);
	} else {
		manager.setFile(QString());
	}

	QList<PTag> m_tagList;
	QExiv2 exiv2;
	if (exiv2.load(fileName)) {
		m_tagList = exiv2.xmpPTags();
		if (m_tagList.isEmpty()) {
			// Check MWG regions
			m_tagList = exiv2.xmpMWG_RegionsTags();
			if (m_tagList.isEmpty()) {
				return 0;
			}
		}
		for (int i = 0; i < m_tagList.size(); i++) {
			qDebug() << m_tagList.at(i).region();
			manager.addRegion(m_tagList.at(i).region(), m_tagList.at(i).name());
		}
	}

	manager.show();
	return app.exec();
}
