TEMPLATE = app
TARGET = regions

include (../../libQExiv2/libQExiv2.pri)
#include (../../krbkacd/MetadataView/MetadataView.pri) \

#-- Places for generated files
#DESTDIR = ../..
OBJECTS_DIR = objs
MOC_DIR = mocs
UI_DIR = ui
RCC_DIR = resources

SOURCES += main.cpp \
	RegionEditManager.cpp \
	ImageGraphicsView.cpp \
	ImageGraphicsItem.cpp

HEADERS += \
	RegionEditManager.h \
	ImageGraphicsItem.h \
	ImageGraphicsView.h


#LIBS += -L../../libQExiv2 -lQExiv2
LIBS += -L$$PWD -lQExiv2

