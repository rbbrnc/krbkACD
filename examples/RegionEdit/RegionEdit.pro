TEMPLATE = app

include (../../libQExiv2/libQExiv2.pri)

#-- Places for generated files
DESTDIR = ../..
OBJECTS_DIR = objs
MOC_DIR = mocs
UI_DIR = ui
RCC_DIR = resources

SOURCES += main.cpp \
    RegionEdit.cpp \
    mwgRegionInfo.cpp


HEADERS += \
    RegionEdit.h \
    mwgRegionInfo.h

LIBS += -L../../libQExiv2 -lQExiv2
