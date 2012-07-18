TEMPLATE = app

include (../krbkACD.pri)

TARGET = krbkACD

#-- Places for generated files
DESTDIR = ../
OBJECTS_DIR = objs
MOC_DIR = mocs
UI_DIR = ui
RCC_DIR = resources

SOURCES += main.cpp \
        mainwindow.cpp \
	browser_page.cpp \
	filelistwidget.cpp \
	metadata_view.cpp \
	work_page.cpp \
	file_data.cpp \
	PTagGraphicsItem.cpp \
	photo_widget.cpp

HEADERS += mainwindow.h \
	browser_page.h \
	filelistwidget.h \
	metadata_view.h \
	work_page.h \
	file_data.h \
	PTagGraphicsItem.h \
	photo_widget.h

FORMS += mainwindow.ui \
	browser_page.ui \
	work_page.ui \
	photo_widget.ui

LIBS += $$EXIV2_LDFLAGS -lmagic -L../libQExiv2 ../libQExiv2/libQExiv2.a
