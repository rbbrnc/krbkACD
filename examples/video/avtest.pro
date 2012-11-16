TEMPLATE = app
TARGET = avtest
DEPENDPATH += .

# Input
SOURCES += main.cpp \
	VideoDecode.cpp \
	AvManager.cpp

#	VideoWidget.cpp \

HEADERS += \
	VideoDecode.h \
	AvManager.h

#	VideoWidget.h \


FORMS += AvManager.ui

QMAKE_CXXFLAGS += -Wall -W -Wunreachable-code -O0 -g -ggdb

INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib -lavcodec -lavformat -lavutil -lswscale -lbz2
