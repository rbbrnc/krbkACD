TEMPLATE = app
TARGET = avtest
DEPENDPATH += .
INCLUDEPATH += /usr/local/include

# Input
SOURCES += main.cpp \
	VideoDecode.cpp \
	VideoWidget.cpp \

HEADERS += \
	VideoDecode.h \
	VideoWidget.h \


QMAKE_CXXFLAGS += -Wall -W -Wunreachable-code -O0 -g -ggdb

LIBS += -L/usr/local/include -lavcodec -lavformat -lavutil -lswscale -lbz2
# -lm -lz

