TEMPLATE = lib

#-- Places for generated files
#DESTDIR = ../libs
OBJECTS_DIR = objs
MOC_DIR = mocs
UI_DIR = ui
RCC_DIR = resources

SOURCES   += SpotlightWidget.cpp
HEADERS   += SpotlightWidget.h
RESOURCES += SpotlightWidget.qrc

CONFIG += staticlib
CONFIG += silent
