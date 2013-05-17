TARGET = QExiv2
TEMPLATE = lib

include ($$PWD/libQExiv2.pri)

#-- Places for generated files
#DESTDIR = ../libs
OBJECTS_DIR = objs
MOC_DIR = mocs
UI_DIR = ui
RCC_DIR = resources

SOURCES += \
	QExiv2.cpp \
	QExiv2_p.cpp \
	XmpRegion.cpp \
	exif.cpp \
	exif_preview.cpp \
	img_comment.cpp \
	iptc.cpp \
	xmp.cpp \
	xmp_regions.cpp

HEADERS += \
	QExiv2.h \
	QExiv2_p.h \
	XmpRegion.h

CONFIG += staticlib
CONFIG += silent
