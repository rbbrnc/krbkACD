TARGET = filemanager
TEMPLATE = app

include (../../libExtraWidget/RenameDialog/RenameDialog.pri)
PRE_TARGETDEPS += $$OUT_PWD/../../libExtraWidget/RenameDialog/lib$${RENAME_DIALOG_LIBNAME}.a

DESTDIR = ../..
SOURCES += main.cpp\
        mainwindow.cpp \
        FileManager.cpp \

HEADERS  += mainwindow.h \
        FileManager.h \

FORMS    += FileManager.ui \

LIBS += -L$$OUT_PWD/../../libExtraWidget/RenameDialog -l$$RENAME_DIALOG_LIBNAME
