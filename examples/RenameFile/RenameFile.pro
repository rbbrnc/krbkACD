TEMPLATE = app

include (../../libExtraWidget/RenameDialog/RenameDialog.pri)

PRE_TARGETDEPS += $$OUT_PWD/../../libExtraWidget/RenameDialog/lib$${RENAME_DIALOG_LIBNAME}.a

#-- Places for generated files
DESTDIR = ../..
OBJECTS_DIR = objs
MOC_DIR = mocs
UI_DIR = ui
RCC_DIR = resources

SOURCES += main.cpp

LIBS += -L$$OUT_PWD/../../libExtraWidget/RenameDialog -l$$RENAME_DIALOG_LIBNAME
