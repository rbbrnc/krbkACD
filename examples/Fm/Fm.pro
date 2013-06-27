TARGET = fm
TEMPLATE = app
#DESTDIR = ../..


include ($$PWD/FileManager/FileManager.pri)

QMAKE_CXXFLAGS += -Wall -W -Wunreachable-code -g -ggdb

CONFIG += silent

SOURCES += \
	main.cpp\
        MainWindow.cpp \

#	FileManager.cpp

HEADERS += \
	MainWindow.h \

#	FileManager.h

FORMS   += MainWindow.ui

