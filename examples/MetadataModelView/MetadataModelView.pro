TEMPLATE = app
TARGET = metadata_treeview

COMMON_PATH=../../common
include ($$COMMON_PATH/libQExiv2/libQExiv2.pri)
include ($$COMMON_PATH/MetadataTreeModel/MetadataTreeModel.pri)
include ($$COMMON_PATH/MetadataTreeModel/MetadataTreeViewPage.pri)

PRE_TARGETDEPS += $$OUT_PWD/$$COMMON_PATH/libQExiv2/libQExiv2.a

#-- Places for generated files
DESTDIR = ../..
OBJECTS_DIR = objs
MOC_DIR = mocs
UI_DIR = ui
RCC_DIR = resources

SOURCES += main.cpp

LIBS += -L$$OUT_PWD/$$COMMON_PATH/libQExiv2 -lQExiv2
CONFIG += silent

