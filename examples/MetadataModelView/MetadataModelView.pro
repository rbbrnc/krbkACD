TEMPLATE = app
TARGET = view

include (../../libQExiv2/libQExiv2.pri)
include (../../krbkacd/MetadataTreeModel/MetadataTreeModel.pri)
include (../../krbkacd/MetadataTreeModel/MetadataTreeViewPage.pri)

#-- Places for generated files
DESTDIR = ../..
OBJECTS_DIR = objs
MOC_DIR = mocs
UI_DIR = ui
RCC_DIR = resources

SOURCES += \
	main.cpp \

#	MetadataTreeViewPage.cpp

#HEADERS += \
#	MetadataTreeViewPage.h

#FORMS += \
#	MetadataTreeViewPage.ui

LIBS += -L../../libQExiv2 -lQExiv2
CONFIG += silent

