TARGET = filemanager
TEMPLATE = app

include (../../libExtraWidget/RenameDialog/RenameDialog.pri)
include (../../libExtraWidget/QMagic/QMagic.pri)
PRE_TARGETDEPS += $$OUT_PWD/../../libExtraWidget/RenameDialog/lib$${RENAME_DIALOG_LIBNAME}.a
PRE_TARGETDEPS += $$OUT_PWD/../../libExtraWidget/QMagic/lib$${QMAGIC_LIBNAME}.a

DESTDIR = ../..
SOURCES += main.cpp\
        MainWindow.cpp \
        FileManager.cpp \

HEADERS  += MainWindow.h \
        FileManager.h \

FORMS += FileManager.ui \
        MainWindow.ui \

LIBS += -L$$OUT_PWD/../../libExtraWidget/RenameDialog -l$$RENAME_DIALOG_LIBNAME
LIBS += -L$$OUT_PWD/../../libExtraWidget/QMagic $$LIB_QMAGIC
