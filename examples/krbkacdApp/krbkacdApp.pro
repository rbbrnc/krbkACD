TARGET = krbkacd
TEMPLATE = app
DESTDIR = ../..

COMMON_PATH=../../common

#-- Rename Dialog
include ($$COMMON_PATH/RenameDialog/RenameDialog.pri)
PRE_TARGETDEPS += $$OUT_PWD/$$COMMON_PATH/RenameDialog/lib$${RENAME_DIALOG_LIBNAME}.a
LIBS += -L$$OUT_PWD/$$COMMON_PATH/RenameDialog -l$$RENAME_DIALOG_LIBNAME

#-- QMagic library
include ($$COMMON_PATH/QMagic/QMagic.pri)
PRE_TARGETDEPS += $$OUT_PWD/$$COMMON_PATH/QMagic/lib$${QMAGIC_LIBNAME}.a
LIBS += -L$$OUT_PWD/$$COMMON_PATH/QMagic $$LIB_QMAGIC

#-- QExiv2 metadata library
include ($$COMMON_PATH/libQExiv2/libQExiv2.pri)
PRE_TARGETDEPS += $$OUT_PWD/$$COMMON_PATH/libQExiv2/libQExiv2.a
LIBS += -L$$OUT_PWD/$$COMMON_PATH/libQExiv2 -lQExiv2

#-- OpevCV lib for object detection
OPENCV_CXXFLAGS = $$system(pkg-config opencv --cflags)
OPENCV_LDFLAGS  = $$system(pkg-config opencv --libs)
QMAKE_CXXFLAGS += $$OPENCV_CXXFLAGS
#-- Only needed libraries
OPENCV_PATH = "/opt/opencv"
LIBS += -L$$OPENCV_PATH/lib -lopencv_core -lopencv_highgui -lopencv_objdetect
#-- Full openCV libraries
#LIBS += $$OPENCV_LDFLAGS


include ($$COMMON_PATH/ImageView/ImageView.pri)
include ($$COMMON_PATH/MetadataTreeModel/MetadataTreeModel.pri)
include ($$COMMON_PATH/MetadataTreeModel/MetadataTreeViewPage.pri)
include ($$COMMON_PATH/FileManager/FileManager.pri)
include ($$COMMON_PATH/LocationDialog/LocationDialog.pri)

QMAKE_CXXFLAGS += -Wall -W -Wunreachable-code -g -ggdb

CONFIG += silent

SOURCES += \
	main.cpp\
	detect.cpp\
        MainWindow.cpp

HEADERS += \
	MainWindow.h \
	detect.cpp

FORMS     += MainWindow.ui
RESOURCES += $$PWD/styles/darkorange/darkorange.qrc

