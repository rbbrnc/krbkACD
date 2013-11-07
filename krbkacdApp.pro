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
include ($$PWD/libQExiv2/DateTimeDialog/DateTimeDialog.pri)

#-- Try clang
#QMAKE_CC  = clang
#QMAKE_CXX = clang++

#-- common gcc flags
QMAKE_CXXFLAGS += -Wall -W -Wunreachable-code
QMAKE_CXXFLAGS += -ffunction-sections -fdata-sections
QMAKE_LFLAGS   += -Wl,--gc-sections,--as-needed

#-- DEBUG Symbols
QMAKE_CXXFLAGS += -g -ggdb
#QMAKE_LFLAGS   += -Wl,--print-gc-sections
#DEFINES += QT_NO_DEBUG_OUTPUT QT_NO_WARNING_OUTPUT

#-- COVERAGE
#QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
#QMAKE_LDFLAGS  += -fprofile-arcs -ftest-coverage
#LIBS += -lgcov

#-- PROFILE
#QMAKE_CXXFLAGS += -pg
#QMAKE_LFLAGS   += -pg

SOURCES += main.cpp\
	   mainwindow.cpp \
	   infowidget.cpp

HEADERS += mainwindow.h \
	   infowidget.h

FORMS   += mainwindow.ui \
	   infowidget.ui

RESOURCES += $$PWD/libQExiv2/QExiv2.qrc

#-- Custom style
#DEFINES += USE_CUSTOM_STYLE
#RESOURCES += $$PWD/styles/darkorange/darkorange.qrc

