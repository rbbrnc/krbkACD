TEMPLATE = app
TARGET = videotest
DEPENDPATH += .

SOURCES += \
	main.cpp \
	VideoDecode.cpp \
	AvManager.cpp

HEADERS += \
	VideoDecode.h \
	AvManager.h

FORMS += \
	AvManager.ui

QMAKE_CXXFLAGS += -Wall -W -Wunreachable-code -O0 -g -ggdb

CONFIG += silent

unix:contains(QT_ARCH, powerpc): {
	INCLUDEPATH += /opt/eldk4.2/ppc_6xx/usr/include
	INCLUDEPATH += /opt/eldk4.2/ppc_6xx/usr/local/include
	LIBS += -L/opt/eldk4.2/ppc_6xx/usr/local -lts
	LIBS += -L/opt/eldk4.2/ppc_6xx/usr/local/lib -lavcodec -lavformat -lavutil -lswscale -lbz2
}

unix:contains(QT_ARCH, i386): {
	INCLUDEPATH += /usr/local/include
	LIBS += -L/usr/local/lib -lavcodec -lavformat -lavutil -lswscale -lbz2
}
