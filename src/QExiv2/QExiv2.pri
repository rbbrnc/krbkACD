INCLUDEPATH += $$PWD
QT += positioning

SOURCES += \
	$$PWD/md_location.cpp \
	$$PWD/md_social.cpp \
	$$PWD/mwg_region.cpp \
	$$PWD/qexiv2.cpp \
	$$PWD/qexiv2_p.cpp \
	$$PWD/qexiv2_datetime.cpp \
	$$PWD/qexiv2_exif.cpp \
	$$PWD/qexiv2_gps.cpp \
	$$PWD/qexiv2_imgcomment.cpp \
	$$PWD/qexiv2_iptc.cpp \
	$$PWD/qexiv2_locations.cpp \
	$$PWD/qexiv2_xmp.cpp \
	$$PWD/qexiv2_xmpregion.cpp \
	$$PWD/qexiv2_xmpsocial.cpp

#	$$PWD/mp_region.cpp \
#	$$PWD/metadatalocation.cpp \
#	$$PWD/metadatalocation.h

HEADERS += \
	$$PWD/QExiv2.h \
	$$PWD/qexiv2_p.h \
	$$PWD/mwg_region.h \
	$$PWD/md_location.h \
	$$PWD/md_social.h

EXIV2_CXXFLAGS = $$system(pkg-config exiv2 --cflags)
EXIV2_LDFLAGS  = $$system(pkg-config exiv2 --libs)

LIBS += $$EXIV2_LDFLAGS
QMAKE_CXXFLAGS += $$EXIV2_CXXFLAGS
