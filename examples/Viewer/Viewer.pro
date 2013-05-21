TARGET = viewer
TEMPLATE = app

COMMON_PATH=../../common

include ($$COMMON_PATH/libImageView/ImageView.pri)

PRE_TARGETDEPS += $$OUT_PWD/$$COMMON_PATH/libImageView/libImageView.a

#-- Places for generated files
DESTDIR = ../..
#OBJECTS_DIR = objs
#MOC_DIR = mocs
#UI_DIR = ui
#RCC_DIR = resources

SOURCES += main.cpp \
	MainWindow.cpp

HEADERS += \
	MainWindow.h

RESOURCES += $$PWD/icons.qrc

#OPENCV_CXXFLAGS = $$system(pkg-config opencv --cflags)
#OPENCV_LDFLAGS  = $$system(pkg-config opencv --libs)
#-- Only needed libraries
#OPENCV_PATH = "/opt/opencv"
#LIBS += -L$$OPENCV_PATH/lib -lopencv_core -lopencv_highgui -lopencv_objdetect
#-- Full openCV libraries
#LIBS += $$OPENCV_LDFLAGS

LIBS += -L$$OUT_PWD/$$COMMON_PATH/libImageView -lImageView

QMAKE_CXXFLAGS += -Wall -W -Wunreachable-code
#QMAKE_CXXFLAGS += $$OPENCV_CXXFLAGS

CONFIG += silent
