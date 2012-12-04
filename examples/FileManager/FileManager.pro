TARGET = filemanager
TEMPLATE = app

include (../../libExtraWidget/RenameDialog/RenameDialog.pri)
include (../../libExtraWidget/QMagic/QMagic.pri)
include (../../libQExiv2/libQExiv2.pri)
include (../../krbkacd/ImageView/ImageView.pri)
include (../../krbkacd/MetadataTreeModel/MetadataTreeModel.pri)

PRE_TARGETDEPS += $$OUT_PWD/../../libExtraWidget/RenameDialog/lib$${RENAME_DIALOG_LIBNAME}.a
PRE_TARGETDEPS += $$OUT_PWD/../../libExtraWidget/QMagic/lib$${QMAGIC_LIBNAME}.a
PRE_TARGETDEPS += $$OUT_PWD/../../libQExiv2/libQExiv2.a

DESTDIR = ../..
SOURCES += main.cpp\
        MainWindow.cpp \
        FileManager.cpp \
        FileManagerPage.cpp \

HEADERS  += MainWindow.h \
        FileManager.h \
        FileManagerPage.h \

FORMS += FileManager.ui \
        MainWindow.ui \
        FileManagerPage.ui \

LIBS += -L$$OUT_PWD/../../libQExiv2 -lQExiv2
LIBS += -L$$OUT_PWD/../../libExtraWidget/RenameDialog -l$$RENAME_DIALOG_LIBNAME
LIBS += -L$$OUT_PWD/../../libExtraWidget/QMagic $$LIB_QMAGIC

CONFIG += silent

