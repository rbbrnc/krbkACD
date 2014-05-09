TEMPLATE = app
TARGET = krkcontacts
QT += sql

CONFIG += silent

#-- Check QT version
message(Qt version: $$[QT_VERSION])
greaterThan(QT_MAJOR_VERSION, 4) {
	QT += widgets
}

#-- Places for generated files
OBJECTS_DIR = objs_$$QT_ARCH
MOC_DIR     = mocs_$$QT_ARCH
UI_DIR      = ui_$$QT_ARCH
RCC_DIR     = rcc_$$QT_ARCH
#----------------------------------

win32 {
    CONFIG += static
    QMAKE_LFLAGS += -static-libgcc
}

DEPENDPATH += .
INCLUDEPATH += .

HEADERS += \
    db.h \
    mainwindow.h \
    insertdialog.h \
    contactsform.h \
    dbcontactsdata.h \

SOURCES += \
    main.cpp \
    db.cpp \
    mainwindow.cpp \
    insertdialog.cpp \
    contactsform.cpp \
    dbcontactsdata.cpp \

FORMS += \
    mainwindow.ui \
    insertdialog.ui \
    contactsform.ui \

RESOURCES += \
    resources.qrc

OTHER_FILES +=
