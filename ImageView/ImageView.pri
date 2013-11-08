INCLUDEPATH += $$PWD

SOURCES += \
	$$PWD/RegionGraphicsItem.cpp \
	$$PWD/ImageGraphicsItem.cpp \
	$$PWD/ImageGraphicsView.cpp \
	$$PWD/ImageViewManager.cpp \
	$$PWD/RegionEditDialog.cpp \

#$$PWD/ObjectDetect.cpp

HEADERS += \
	$$PWD/RegionGraphicsItem.h \
	$$PWD/ImageGraphicsItem.h \
	$$PWD/ImageGraphicsView.h \
	$$PWD/ImageViewManager.h \
	$$PWD/RegionEditDialog.h \

#$$PWD/ObjectDetect.h

FORMS += \
	$$PWD/RegionEditDialog.ui

RESOURCES += $$PWD/ImageViewManager.qrc

#------------------------------------------------------------------------------
#-- OpevCV lib for object detection
#------------------------------------------------------------------------------
system(pkg-config opencv):PLUGINS += object_detect
contains(PLUGINS, object_detect) {
	message("Found OpenCV $$system(pkg-config opencv --modversion) --> Configuring 'object_detect' module..." )

	OPENCV_PREFIX_PATH = $$system(pkg-config opencv --variable=prefix)
	OPENCV_HAARCASCADE = "$$OPENCV_PREFIX_PATH/share/OpenCV/haarcascades/haarcascade_frontalface_alt_tree.xml"
	#OPENCV_HAARCASCADE = "$$OPENCV_PREFIX_PATH/share/OpenCV/haarcascades/haarcascade_frontalface_default.xml"

	!exists($$OPENCV_HAARCASCADE) {
		error(OpenCv Haar Cascade File Missing: $$OPENCV_HAARCASCADE)
	}

	message("Using OpenCV Haar Cascade: $$OPENCV_HAARCASCADE")
	DEFINES += OPENCV_HAARCASCADE=\'\"$$OPENCV_HAARCASCADE\"\'

	OPENCV_CXXFLAGS = $$system(pkg-config opencv --cflags)
	OPENCV_LDFLAGS  = $$system(pkg-config opencv --libs)
	QMAKE_CXXFLAGS += $$OPENCV_CXXFLAGS

	#-- Link needed libraries only!!
	LIBS += -L$$OPENCV_PREFIX_PATH/lib -lopencv_core -lopencv_highgui -lopencv_objdetect

	#-- Link all OpenCV libraries
	#LIBS += $$OPENCV_LDFLAGS

	SOURCES += $$PWD/ObjectDetect.cpp
	HEADERS += $$PWD/ObjectDetect.h
	DEFINES += ENABLE_OBJECT_DETECT_MODULE
}

