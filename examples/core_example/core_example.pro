TEMPLATE = app

include (../../krbkACD.pri)

#-- Places for generated files
DESTDIR = ../..
OBJECTS_DIR = objs
MOC_DIR = mocs
UI_DIR = ui
RCC_DIR = resources

SOURCES += main.cpp

#LIBS += $$EXIV2_LDFLAGS -lmagic
LIBS += $$EXIV2_LDFLAGS -L../../libQExiv2 ../../libQExiv2/libQExiv2.a
