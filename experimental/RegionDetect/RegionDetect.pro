TEMPLATE = app

OPENCV_CXXFLAGS = $$system(pkg-config opencv --cflags)
OPENCV_LDFLAGS  = $$system(pkg-config opencv --libs)

#-- Places for generated files
#DESTDIR = ../..
OBJECTS_DIR = objs
MOC_DIR = mocs
UI_DIR = ui
RCC_DIR = resources

SOURCES += main.cpp \
	detect.cpp \
	mwgRegion.cpp \
	mwgRegionList.cpp

HEADERS += \
	detect.h \
	mwgRegion.h \
	mwgRegionList.h

#-- Only needed libraries
#OPENCV_PATH = "/opt/opencv"
#LIBS += -L$$OPENCV_PATH/lib -lopencv_core -lopencv_highgui -lopencv_objdetect
#-- Full openCV libraries
LIBS += $$OPENCV_LDFLAGS
QMAKE_CXXFLAGS += -Wall -W -Wunreachable-code
QMAKE_CXXFLAGS += $$OPENCV_CXXFLAGS

CONFIG += silent

