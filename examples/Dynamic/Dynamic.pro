TEMPLATE = app

#-- Places for generated files
#DESTDIR = ../..
OBJECTS_DIR = objs
MOC_DIR = mocs
UI_DIR = ui
RCC_DIR = resources

SOURCES += main.cpp \
    dynamic.cpp


HEADERS += \
    dynamic.h

RESOURCES += \
	$$PWD/Dynamic.qrc

