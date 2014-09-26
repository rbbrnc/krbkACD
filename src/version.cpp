#include "version.h"
#include "build_date.h"

#ifdef ENABLE_OBJECT_DETECT_MODULE
#include <opencv/cv.h>
#else
#define CV_VERSION "<not linked>"
#endif

// The pragmas are required to be able to catch exceptions thrown by libexiv2:
// See http://gcc.gnu.org/wiki/Visibility, the section about c++ exceptions.
// They are needed for all libexiv2 versions that do not care about visibility.
#ifdef __GNUC__
#pragma GCC visibility push(default)
#endif

#include <exiv2/exiv2.hpp>

#ifdef __GNUC__
#pragma GCC visibility pop
#endif

namespace KrbkACDVersion {

const QString exiv2VersionString()
{
    return QString(Exiv2::version());
}

const QString openCvVersionString()
{
    return QString(CV_VERSION);
}

const QString krbkAcdVersionString()
{
	const QString version = QString("%1 [%2]").arg(KRBKACD_VERSION).arg(BUILD_DATE);
    return QString(version);
}

} // end-namespace
