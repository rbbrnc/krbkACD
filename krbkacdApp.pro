TARGET = krbkacd
TEMPLATE = app
CONFIG += silent

#-- Places for generated files
#DESTDIR = .
OBJECTS_DIR = out/objs
MOC_DIR     = out/mocs
UI_DIR      = out/ui
RCC_DIR     = out/resources

include ($$PWD/SpotlightWidget/SpotlightWidget.pri)

include ($$PWD/FileManager/FileManager.pri)
include ($$PWD/ImageView/ImageView.pri)
include ($$PWD/QMagic/QMagic.pri)

include ($$PWD/libQExiv2/libQExiv2.pri)
include ($$PWD/libQExiv2/MetadataTree/MetadataTree.pri)
include ($$PWD/libQExiv2/MetadataDialog/MetadataDialog.pri)
include ($$PWD/libQExiv2/LocationDialog/LocationDialog.pri)
include ($$PWD/libQExiv2/SocialDialog/SocialMetadataDialog.pri)

QMAKE_CXXFLAGS += -Wall -W -Wunreachable-code -g -ggdb

SOURCES += \
	main.cpp\
	mainwindow.cpp \
    infowidget.cpp

HEADERS += mainwindow.h \
    infowidget.h
FORMS   += mainwindow.ui \
    infowidget.ui


RESOURCES += \
	$$PWD/styles/darkorange/darkorange.qrc \
	$$PWD/libQExiv2/QExiv2.qrc

