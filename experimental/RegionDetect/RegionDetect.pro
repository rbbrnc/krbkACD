TEMPLATE = app

OPENCV_PATH = "/opt/opencv"
INCLUDEPATH += $$OPENCV_PATH/include

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

LIBS += -L$$OPENCV_PATH/lib -lopencv_core -lopencv_highgui -lopencv_objdetect
CONFIG += silent

