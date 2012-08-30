TEMPLATE = app

include (../../krbkACD.pri)

#-- Places for generated files
DESTDIR = ../..
OBJECTS_DIR = objs
MOC_DIR = mocs
UI_DIR = ui
RCC_DIR = resources

SOURCES += main.cpp \
	spotlight_widget.cpp \
        MainWindow.cpp

HEADERS += MainWindow.h \
	spotlight_widget.h \

FORMS     += MainWindow.ui
RESOURCES += spotlight_widget.qrc

LIBS += $$EXIV2_LDFLAGS -L../../libQExiv2 ../../libQExiv2/libQExiv2.a

