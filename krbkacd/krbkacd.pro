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
	lens_zoom.cpp \
	filelistwidget.cpp \
	exiv2_view.cpp \
	fullscreen_page.cpp \
	work_page.cpp \
	file_data.cpp \
	PTagWidget.cpp \
    photo_widget.cpp

HEADERS += mainwindow.h \
	browser_page.h \
	lens_zoom.h \
	filelistwidget.h \
	exiv2_view.h \
	fullscreen_page.h \
	work_page.h \
	file_data.h \
	PTagWidget.h \
    photo_widget.h

FORMS += mainwindow.ui \
	browser_page.ui \
	fullscreen_page.ui \
	work_page.ui \
    photo_widget.ui

LIBS += $$EXIV2_LDFLAGS -lmagic -L../libQExiv2 ../libQExiv2/libQExiv2.a
