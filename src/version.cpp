#include "version.h"

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

const QString exiv2VerisionString()
{
    return QString(Exiv2::version());
}

const QString openCvVerisionString()
{
    return QString(CV_VERSION);
}

} // end-namespace
