TEMPLATE = app

include (../../krbkACD.pri)
include (../../libExtraWidget/SpotlightWidget/SpotlightWidget.pri)

#-- Places for generated files
DESTDIR = ../..
OBJECTS_DIR = objs
MOC_DIR = mocs
UI_DIR = ui
RCC_DIR = resources

SOURCES += main.cpp MainWindow.cpp
HEADERS += MainWindow.h
FORMS   += MainWindow.ui

LIBS += $$EXIV2_LDFLAGS -L../../libQExiv2 ../../libQExiv2/libQExiv2.a
LIBS += -L$$OUT_PWD/../../libExtraWidget/SpotlightWidget -l$$SPOTLIGHT_WIDGET_LIBNAME

