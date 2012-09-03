TEMPLATE = app

include (../../krbkACD.pri)

#-- Places for generated files
DESTDIR = ../..
OBJECTS_DIR = objs
MOC_DIR = mocs
UI_DIR = ui
RCC_DIR = resources

SOURCES += main.cpp \
	MetadataItem.cpp \
	MetadataModel.cpp \

HEADERS += MetadataDialog.h \
	MetadataItem.h \
	MetadataModel.h

LIBS += $$EXIV2_LDFLAGS
LIBS += -L../../libQExiv2 ../../libQExiv2/libQExiv2.a

