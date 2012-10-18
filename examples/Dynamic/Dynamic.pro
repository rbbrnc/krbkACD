TEMPLATE = app

#-- Places for generated files
#DESTDIR = ../..
OBJECTS_DIR = objs
MOC_DIR = mocs
UI_DIR = ui
RCC_DIR = resources

SOURCES += main.cpp \
    PatternWidget.cpp \
    RenameDialog.cpp

HEADERS += \
    PatternWidget.h \
    RenameDialog.h

RESOURCES += \
	$$PWD/Dynamic.qrc

FORMS += \
    RenameDialog.ui

