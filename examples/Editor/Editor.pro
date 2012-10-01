TEMPLATE = app

include (../../krbkACD.pri)
include (../../libExtraWidget/SpotlightWidget/SpotlightWidget.pri)

#-- Places for generated files
DESTDIR = ../..
OBJECTS_DIR = objs
MOC_DIR = mocs
UI_DIR = ui
RCC_DIR = resources

INCLUDEPATH += ../../krbkacd

SOURCES += main.cpp \
	MetadataDialog.cpp \
	../../krbkacd/FileGeneralInfo.cpp \
	../../krbkacd/MetadataEdit.cpp \
	../../krbkacd/MetadataRegionEdit.cpp \
	../../krbkacd/MetadataLocation.cpp \

HEADERS += MetadataDialog.h \
	../../krbkacd/FileGeneralInfo.h \
	../../krbkacd/MetadataEdit.h \
	../../krbkacd/MetadataRegionEdit.h \
	../../krbkacd/MetadataLocation.h \

FORMS   += \
	../../krbkacd/FileGeneralInfo.ui \
	../../krbkacd/MetadataEdit.ui \
	../../krbkacd/MetadataRegionEdit.ui \
	../../krbkacd/MetadataLocation.ui \


LIBS += $$EXIV2_LDFLAGS
LIBS += -L../../libQExiv2 ../../libQExiv2/libQExiv2.a
LIBS += -L../../libExtraWidget/SpotlightWidget -l$$SPOTLIGHT_WIDGET_LIBNAME

