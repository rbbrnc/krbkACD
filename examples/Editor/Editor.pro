TEMPLATE = app

COMMON_PATH=../../common

include ($$COMMON_PATH/libQExiv2/libQExiv2.pri)
include ($$COMMON_PATH/SpotlightWidget/SpotlightWidget.pri)
include ($$COMMON_PATH/MetadataEditor/MetadataEditForm.pri)

#-- Places for generated files
DESTDIR = ../..
OBJECTS_DIR = objs
MOC_DIR = mocs
UI_DIR = ui
RCC_DIR = resources

SOURCES += main.cpp

LIBS += -L$$OUT_PWD/$$COMMON_PATH/libQExiv2 -lQExiv2
LIBS += -L$$OUT_PWD/$$COMMON_PATH/SpotlightWidget -l$$SPOTLIGHT_WIDGET_LIBNAME

CONFIG += silent
