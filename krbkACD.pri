
INCLUDEPATH += $$PWD/libQExiv2

EXIV2_CXXFLAGS = $$system(pkg-config exiv2 --cflags)
EXIV2_LDFLAGS  = $$system(pkg-config exiv2 --libs)

QMAKE_CXXFLAGS += -Wall -W -Wunreachable-code
