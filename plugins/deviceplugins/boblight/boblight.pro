include(../../plugins.pri)

TARGET = $$qtLibraryTarget(guh_devicepluginboblight)

INCLUDEPATH += /usr/local/include/
LIBS += -L/opt/beaglebone-black/rootfs/usr/local/lib/ -lboblight

SOURCES += \
    devicepluginboblight.cpp \
    bobclient.cpp \
    coloranimation.cpp

HEADERS += \
    devicepluginboblight.h \
    bobclient.h \
    coloranimation.h



