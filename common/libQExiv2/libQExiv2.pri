INCLUDEPATH += $$PWD

EXIV2_CXXFLAGS = $$system(pkg-config exiv2 --cflags)
EXIV2_LDFLAGS  = $$system(pkg-config exiv2 --libs)

LIBS += $$EXIV2_LDFLAGS
QMAKE_CXXFLAGS += -Wall -W -Wunreachable-code
QMAKE_CXXFLAGS += $$EXIV2_CXXFLAGS
