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

INCLUDEPATH += FindDuplicates

SOURCES += main.cpp \
        mainwindow.cpp \
	filelistwidget.cpp \
	file_data.cpp \
	PTagGraphicsItem.cpp \
	photo_widget.cpp \
	browser_page.cpp \
	file_utils.cpp \
	MetadataTreeModel/MetadataTreeItem.cpp \
	MetadataTreeModel/MetadataTreeModel.cpp \
	FindDuplicates/work_page.cpp \
	FindDuplicates/duplicate_page.cpp \
	FindDuplicates/histogram.cpp \
	FindDuplicates/thumb_view.cpp \

HEADERS += mainwindow.h \
	pages.h \
	browser_page.h \
	filelistwidget.h \
	file_data.h \
	PTagGraphicsItem.h \
	photo_widget.h \
	file_utils.h \
	MetadataTreeModel/MetadataTreeItem.h \
	MetadataTreeModel/MetadataTreeModel.h \
	FindDuplicates/work_page.h \
	FindDuplicates/duplicate_page.h \
	FindDuplicates/histogram.h \
	FindDuplicates/thumb_view.h \

FORMS += mainwindow.ui \
	browser_page.ui \
	photo_widget.ui \
	FindDuplicates/work_page.ui \
	FindDuplicates/duplicate_page.ui

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
