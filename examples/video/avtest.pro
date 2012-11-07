TEMPLATE = app
TARGET = avtest
DEPENDPATH += .

# Input
SOURCES += main.cpp \
	VideoDecode.cpp \
	VideoWidget.cpp \

HEADERS += \
	VideoDecode.h \
	VideoWidget.h \

QMAKE_CXXFLAGS += -Wall -W -Wunreachable-code -O0 -g -ggdb

INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib -lavcodec -lavformat -lavutil -lswscale -lbz2
