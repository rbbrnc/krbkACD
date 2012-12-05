TEMPLATE = app

COMMON_PATH=../../common
include ($$COMMON_PATH/SpotlightWidget/SpotlightWidget.pri)

#-- Places for generated files
DESTDIR = ../..
OBJECTS_DIR = objs
MOC_DIR = mocs
UI_DIR = ui
RCC_DIR = resources

SOURCES += main.cpp MainWindow.cpp
HEADERS += MainWindow.h
FORMS   += MainWindow.ui

LIBS += -L$$OUT_PWD/$$COMMON_PATH/SpotlightWidget -l$$SPOTLIGHT_WIDGET_LIBNAME

CONFIG += silent
