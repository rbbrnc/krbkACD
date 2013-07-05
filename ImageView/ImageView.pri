INCLUDEPATH += $$PWD

SOURCES += \
	$$PWD/RegionGraphicsItem.cpp \
	$$PWD/ImageGraphicsItem.cpp \
	$$PWD/ImageGraphicsView.cpp \
	$$PWD/ImageViewManager.cpp \
	$$PWD/RegionEditDialog.cpp \
	$$PWD/ObjectDetect.cpp

HEADERS += \
	$$PWD/RegionGraphicsItem.h \
	$$PWD/ImageGraphicsItem.h \
	$$PWD/ImageGraphicsView.h \
	$$PWD/ImageViewManager.h \
	$$PWD/RegionEditDialog.h \
	$$PWD/ObjectDetect.h

FORMS += \
	$$PWD/RegionEditDialog.ui

RESOURCES += $$PWD/ImageViewManager.qrc

#-- OpevCV lib for object detection
OPENCV_CXXFLAGS = $$system(pkg-config opencv --cflags)
OPENCV_LDFLAGS  = $$system(pkg-config opencv --libs)
QMAKE_CXXFLAGS += $$OPENCV_CXXFLAGS
#-- Only needed libraries
OPENCV_PATH = "/opt/opencv"
LIBS += -L$$OPENCV_PATH/lib -lopencv_core -lopencv_highgui -lopencv_objdetect
#-- Full openCV libraries
#LIBS += $$OPENCV_LDFLAGS
