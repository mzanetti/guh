include(../../plugins.pri)

TARGET = $$qtLibraryTarget(guh_devicepluginboblight)

INCLUDEPATH += /usr/local/include/
LIBS += -L/usr/local/lib/ -lboblight

SOURCES += \
    devicepluginboblight.cpp \
    bobclient.cpp \
    coloranimation.cpp

HEADERS += \
    devicepluginboblight.h \
    bobclient.h \
    coloranimation.h



