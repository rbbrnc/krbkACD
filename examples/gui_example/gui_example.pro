TEMPLATE = app

include (../../libQExiv2/libQExiv2.pri)

#-- Places for generated files
DESTDIR = ../..
OBJECTS_DIR = objs
MOC_DIR = mocs
UI_DIR = ui
RCC_DIR = resources

SOURCES += main_gui.cpp \
	   widget.cpp \
	   PTagWidget.cpp \

HEADERS += widget.h \
	   PTagWidget.h \

LIBS += -L../../libQExiv2 -lQExiv2
CONFIG += silent

