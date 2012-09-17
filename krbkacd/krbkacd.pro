TEMPLATE = app
POST_TARGETDEPS += ../libQExiv2/libQExiv2.a

include (../krbkACD.pri)
include (../../libExtraWidget/SpotlightWidget/SpotlightWidget.pri)

TARGET = krbkACD

#-- Places for generated files
DESTDIR = ../
OBJECTS_DIR = objs
MOC_DIR = mocs
UI_DIR = ui
RCC_DIR = resources

SOURCES += main.cpp \
        mainwindow.cpp \
	filelistwidget.cpp \
	metadata_view.cpp \
	file_data.cpp \
	PTagGraphicsItem.cpp \
	photo_widget.cpp \
	browser_page.cpp \
	work_page.cpp \
	duplicate_page.cpp \
	histogram.cpp \
	thumb_view.cpp \
	file_utils.cpp \

HEADERS += mainwindow.h \
	pages.h \
	browser_page.h \
	filelistwidget.h \
	metadata_view.h \
	work_page.h \
	file_data.h \
	PTagGraphicsItem.h \
	photo_widget.h \
	histogram.h \
	thumb_view.h \
	file_utils.h \
	duplicate_page.h

FORMS += mainwindow.ui \
	browser_page.ui \
	work_page.ui \
	photo_widget.ui \
	duplicate_page.ui

SOURCES += \
	FileGeneralInfo.cpp \
	MetadataDialog.cpp \
	MetadataEdit.cpp \
	MetadataRegionEdit.cpp

HEADERS += MetadataDialog.h \
	FileGeneralInfo.h \
	MetadataEdit.h \
	MetadataRegionEdit.h

FORMS += MetadataEdit.ui \
	MetadataRegionEdit.ui \
	FileGeneralInfo.ui

LIBS += $$EXIV2_LDFLAGS -lmagic -L../libQExiv2 ../libQExiv2/libQExiv2.a
LIBS += -L../libExtraWidget/SpotlightWidget -l$$SPOTLIGHT_WIDGET_LIBNAME
