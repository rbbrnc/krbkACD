TEMPLATE = app

#-- Places for generated files
#DESTDIR = ../..
OBJECTS_DIR = objs
MOC_DIR = mocs
UI_DIR = ui
RCC_DIR = resources

SOURCES += main.cpp \
	SingleFileRenameDialog.cpp \
    MultiFileRenameDialog.cpp


HEADERS += \
	SingleFileRenameDialog.h \
    MultiFileRenameDialog.h


FORMS += \
    SingleFileRenameDialog.ui \
    MultiFileRenameDialog.ui



