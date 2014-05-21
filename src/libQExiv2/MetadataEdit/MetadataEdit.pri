INCLUDEPATH += $$PWD

SOURCES += \
	$$PWD/metadataeditpage.cpp \
	$$PWD/datetimedialog.cpp \
	$$PWD/keywordsdialog.cpp \
	$$PWD/metadatadialog.cpp

HEADERS += \
	$$PWD/metadataeditpage.h \
	$$PWD/datetimedialog.h \
	$$PWD/keywordsdialog.h \
	$$PWD/metadatadialog.h

FORMS += \
	$$PWD/metadataeditpage.ui \
	$$PWD/datetimedialog.ui \
	$$PWD/keywordsdialog.ui \
	$$PWD/metadatadialog.ui

#-- 
SOURCES += $$PWD/SocialMetadataDialog.cpp
HEADERS += $$PWD/SocialMetadataDialog.h
FORMS   += $$PWD/SocialMetadataDialog.ui
