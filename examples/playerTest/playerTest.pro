TEMPLATE = app
OBJECTS_DIR = objs
MOC_DIR = mocs
UI_DIR = ui
RCC_DIR = resources

DESTDIR = ../..
SOURCES += main.cpp \
        mainwindow.cpp

HEADERS += mainwindow.h

FORMS += mainwindow.ui

CONFIG += silent
