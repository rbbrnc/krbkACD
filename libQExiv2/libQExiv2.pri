INCLUDEPATH += $$PWD

SOURCES += \
	$$PWD/QExiv2.cpp \
	$$PWD/QExiv2_p.cpp \
	$$PWD/exif.cpp \
	$$PWD/exif_preview.cpp \
	$$PWD/img_comment.cpp \
	$$PWD/iptc.cpp \
	$$PWD/xmp.cpp \
	$$PWD/mwg_region.cpp \
	$$PWD/xmp_regions.cpp \

#	$$PWD/mwg_region_list.cpp \
#	$$PWD/mp_region.cpp \

HEADERS += \
	$$PWD/QExiv2.h \
	$$PWD/QExiv2_p.h \
	$$PWD/mwg_region.h \

#	$$PWD/mwg_region_list.h \

EXIV2_CXXFLAGS = $$system(pkg-config exiv2 --cflags)
EXIV2_LDFLAGS  = $$system(pkg-config exiv2 --libs)

LIBS += $$EXIV2_LDFLAGS
QMAKE_CXXFLAGS += $$EXIV2_CXXFLAGS
