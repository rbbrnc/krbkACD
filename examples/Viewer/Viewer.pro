TARGET = viewer
TEMPLATE = app

COMMON_PATH=../../common

include ($$COMMON_PATH/ImageView/ImageView.pri)

include ($$COMMON_PATH/libQExiv2/libQExiv2.pri)
PRE_TARGETDEPS += $$OUT_PWD/$$COMMON_PATH/libQExiv2/libQExiv2.a
LIBS += -L$$OUT_PWD/$$COMMON_PATH/libQExiv2 -lQExiv2

DESTDIR = ../..

SOURCES += main.cpp

QMAKE_CXXFLAGS += -Wall -W -Wunreachable-code

CONFIG += silent

