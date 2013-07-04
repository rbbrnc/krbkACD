TARGET = krbkacd
TEMPLATE = app
CONFIG += silent

#-- Places for generated files
#DESTDIR = .
OBJECTS_DIR = out/objs
MOC_DIR     = out/mocs
UI_DIR      = out/ui
RCC_DIR     = out/resources

include ($$PWD/FileManager/FileManager.pri)
#include ($$PWD/RenameDialog/RenameDialog.pri)
include ($$PWD/QMagic/QMagic.pri)
include ($$PWD/libQExiv2/libQExiv2.pri)
include ($$PWD/ImageView/ImageView.pri)
include ($$PWD/MetadataTree/MetadataTree.pri)
include ($$PWD/LocationDialog/LocationDialog.pri)

QMAKE_CXXFLAGS += -Wall -W -Wunreachable-code -g -ggdb

SOURCES += \
	main.cpp\
	mainwindow.cpp

HEADERS += mainwindow.h
FORMS   += mainwindow.ui


RESOURCES += $$PWD/styles/darkorange/darkorange.qrc

