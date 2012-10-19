TEMPLATE = lib

#-- Places for generated files
#DESTDIR = ../..
OBJECTS_DIR = objs
MOC_DIR = mocs
UI_DIR = ui
RCC_DIR = resources

SOURCES += PatternWidget.cpp \
	RenameDialog.cpp

HEADERS += PatternWidget.h \
	RenameDialog.h

RESOURCES += $$PWD/RenameDialog.qrc
FORMS     += RenameDialog.ui

CONFIG += static

