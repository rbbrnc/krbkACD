TEMPLATE = app

include (../../krbkACD.pri)
include (../../libExtraWidget/SpotlightWidget/SpotlightWidget.pri)

#-- Places for generated files
DESTDIR = ../..
OBJECTS_DIR = objs
MOC_DIR = mocs
UI_DIR = ui
RCC_DIR = resources

SOURCES += main.cpp \
	FileGeneralInfo.cpp \
	MetadataDialog.cpp \
	MetadataEdit.cpp \
	MetadataRegionEdit.cpp

HEADERS += MetadataDialog.h \
	FileGeneralInfo.h \
	MetadataEdit.h \
	MetadataRegionEdit.h

FORMS   += MetadataEdit.ui \
	MetadataRegionEdit.ui \
	FileGeneralInfo.ui

LIBS += $$EXIV2_LDFLAGS
LIBS += -L../../libQExiv2 ../../libQExiv2/libQExiv2.a
LIBS += -L../../libExtraWidget/SpotlightWidget -l$$SPOTLIGHT_WIDGET_LIBNAME

