#include <QDebug>

#include "QExiv2_p.h"

void QExiv2DataPrivate::printExiv2ExceptionError(const QString &msg, Exiv2::Error &e) const
{
	std::string s(e.what());
	qDebug() << msg.toAscii().constData()
		 << "(Error #" << e.code() << ":" << s.c_str() << ")";
}
