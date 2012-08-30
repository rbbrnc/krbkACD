TEMPLATE = app

include (../../krbkACD.pri)

#-- Places for generated files
DESTDIR = ../..
OBJECTS_DIR = objs
MOC_DIR = mocs
UI_DIR = ui
RCC_DIR = resources

SOURCES += main.cpp \
        TagEditor.cpp

HEADERS += TagEditor.h \

FORMS     += TagEditor.ui
#RESOURCES +=

LIBS += $$EXIV2_LDFLAGS -L../../libQExiv2 ../../libQExiv2/libQExiv2.a

