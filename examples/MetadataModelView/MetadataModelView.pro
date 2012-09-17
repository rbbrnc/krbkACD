TEMPLATE = app
TARGET = view

include (../../krbkACD.pri)

#-- Places for generated files
DESTDIR = ../..
OBJECTS_DIR = objs
MOC_DIR = mocs
UI_DIR = ui
RCC_DIR = resources

INCLUDEPATH += ../../krbkacd/MetadataTreeModel

SOURCES += main.cpp \
	../../krbkacd/MetadataTreeModel/MetadataTreeItem.cpp \
	../../krbkacd/MetadataTreeModel/MetadataTreeModel.cpp \

HEADERS += \
	../../krbkacd/MetadataTreeModel/MetadataTreeItem.h \
	../../krbkacd/MetadataTreeModel/MetadataTreeModel.h

LIBS += $$EXIV2_LDFLAGS
LIBS += -L../../libQExiv2 ../../libQExiv2/libQExiv2.a

