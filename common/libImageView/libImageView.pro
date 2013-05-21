TARGET = ImageView
TEMPLATE = lib

include ($$PWD/ImageView.pri)

#-- Places for generated files
#DESTDIR = ..
OBJECTS_DIR = objs
MOC_DIR = mocs
UI_DIR = ui
RCC_DIR = resources

SOURCES += \
	$$PWD/ImageGraphicsItem.cpp \
	$$PWD/ImageGraphicsView.cpp \
	$$PWD/ImageViewManager.cpp \
	$$PWD/RegionGraphicsItem.cpp \
	$$PWD/RegionEditDialog.cpp

HEADERS += \
	$$PWD/ImageGraphicsItem.h \
	$$PWD/ImageGraphicsView.h \
	$$PWD/ImageViewManager.h \
	$$PWD/RegionGraphicsItem.h \
	$$PWD/RegionEditDialog.h

FORMS += \
	$$PWD/RegionEditDialog.ui

CONFIG += staticlib
CONFIG += silent

