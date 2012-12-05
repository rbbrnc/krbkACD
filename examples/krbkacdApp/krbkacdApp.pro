TARGET = krbkacd
TEMPLATE = app

COMMON_PATH=../../common

include ($$COMMON_PATH/RenameDialog/RenameDialog.pri)
include ($$COMMON_PATH/QMagic/QMagic.pri)
include ($$COMMON_PATH/libQExiv2/libQExiv2.pri)
include ($$COMMON_PATH/ImageView/ImageView.pri)
include ($$COMMON_PATH/MetadataTreeModel/MetadataTreeModel.pri)
include ($$COMMON_PATH/MetadataTreeModel/MetadataTreeViewPage.pri)
include ($$COMMON_PATH/FileManager/FileManager.pri)

PRE_TARGETDEPS += $$OUT_PWD/$$COMMON_PATH/RenameDialog/lib$${RENAME_DIALOG_LIBNAME}.a
PRE_TARGETDEPS += $$OUT_PWD/$$COMMON_PATH/QMagic/lib$${QMAGIC_LIBNAME}.a
PRE_TARGETDEPS += $$OUT_PWD/$$COMMON_PATH/libQExiv2/libQExiv2.a

DESTDIR = ../..
SOURCES += \
	main.cpp\
        MainWindow.cpp

HEADERS += MainWindow.h
FORMS   += MainWindow.ui

LIBS += -L$$OUT_PWD/$$COMMON_PATH/libQExiv2 -lQExiv2
LIBS += -L$$OUT_PWD/$$COMMON_PATH/RenameDialog -l$$RENAME_DIALOG_LIBNAME
LIBS += -L$$OUT_PWD/$$COMMON_PATH/QMagic $$LIB_QMAGIC

CONFIG += silent

