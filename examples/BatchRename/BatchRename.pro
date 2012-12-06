TEMPLATE = app

COMMON_PATH=../../common
include ($$COMMON_PATH/RenameDialog/RenameDialog.pri)
PRE_TARGETDEPS += $$OUT_PWD/$$COMMON_PATH/RenameDialog/lib$${RENAME_DIALOG_LIBNAME}.a

#-- Places for generated files
DESTDIR = ../..
OBJECTS_DIR = objs
MOC_DIR = mocs
UI_DIR = ui
RCC_DIR = resources

SOURCES += main.cpp

LIBS += -L$$OUT_PWD/$$COMMON_PATH/RenameDialog -l$$RENAME_DIALOG_LIBNAME

CONFIG += silent
