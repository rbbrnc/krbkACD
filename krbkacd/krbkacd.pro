TEMPLATE = app
POST_TARGETDEPS += ../libQExiv2/libQExiv2.a

include (../../libQExiv2/libQExiv2.pri)
include (../../libExtraWidget/SpotlightWidget/SpotlightWidget.pri)
include (MetadataTreeModel/MetadataTreeModel.pri)
include (FindDuplicates/FindDuplicates.pri)
include (ImageView/ImageView.pri)

TARGET = krbkACD

#-- Places for generated files
DESTDIR = ../
OBJECTS_DIR = objs
MOC_DIR = mocs
UI_DIR  = ui
RCC_DIR = resources

SOURCES += main.cpp \
        mainwindow.cpp \
	filelistwidget.cpp \
	file_data.cpp \
	PTagGraphicsItem.cpp \
	browser_page.cpp \
	file_utils.cpp \
	FileGeneralInfo.cpp \
	MetadataDialog.cpp \
	MetadataEdit.cpp \
	MetadataLocation.cpp \
	MetadataRegionEdit.cpp

HEADERS += mainwindow.h \
	pages.h \
	browser_page.h \
	filelistwidget.h \
	file_data.h \
	PTagGraphicsItem.h \
	file_utils.h \
	MetadataDialog.h \
	FileGeneralInfo.h \
	MetadataEdit.h \
	MetadataLocation.h \
	MetadataRegionEdit.h

FORMS += mainwindow.ui \
	browser_page.ui \
	MetadataEdit.ui \
	MetadataRegionEdit.ui \
	MetadataLocation.ui \
	FileGeneralInfo.ui

LIBS += -lmagic -L../libQExiv2 -lQExiv2
LIBS += -L../libExtraWidget/SpotlightWidget -l$$SPOTLIGHT_WIDGET_LIBNAME
