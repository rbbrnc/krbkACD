TEMPLATE = app

COMMON_PATH=../../common
include ($$COMMON_PATH/libQExiv2/libQExiv2.pri)

PRE_TARGETDEPS += $$OUT_PWD/$$COMMON_PATH/libQExiv2/libQExiv2.a

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

LIBS += -L$$OUT_PWD/$$COMMON_PATH/libQExiv2 -lQExiv2
CONFIG += silent

