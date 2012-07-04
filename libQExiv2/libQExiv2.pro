TARGET = QExiv2
TEMPLATE = lib

#-- Places for generated files
#DESTDIR = ../libs
OBJECTS_DIR = objs
MOC_DIR = mocs
UI_DIR = ui
RCC_DIR = resources

SOURCES += QExiv2.cpp \
	   QExiv2_p.cpp \
	   PTag.cpp

HEADERS += QExiv2.h \
	   QExiv2_p.h \
	   PTag.h \
	   PTag_p.h \

CONFIG += staticlib

EXIV2_CXXFLAGS = $$system(pkg-config exiv2 --cflags)
EXIV2_LDFLAGS  = $$system(pkg-config exiv2 --libs)

LIBS += $$EXIV2_LDFLAGS
QMAKE_CXXFLAGS += -Wall -W $$EXIV2_CXXFLAGS
