TARGET = ImageView
TEMPLATE = lib

include ($$PWD/ImageView.pri)
include ($$PWD/libImageView.pri)

#-- Places for generated files
#DESTDIR = ..
OBJECTS_DIR = objs
MOC_DIR = mocs
UI_DIR = ui
RCC_DIR = resources

CONFIG += staticlib
CONFIG += silent

