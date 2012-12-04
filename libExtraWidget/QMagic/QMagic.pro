TEMPLATE = lib

#-- Places for generated files
#DESTDIR = ../..
OBJECTS_DIR = objs
MOC_DIR = mocs
UI_DIR = ui
RCC_DIR = resources

SOURCES += QMagic.cpp
HEADERS += QMagic.h

#RESOURCES += $$PWD/QMagic.qrc

CONFIG += static
CONFIG += silent

