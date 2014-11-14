include(../../plugins.pri)

TARGET = $$qtLibraryTarget(guh_devicepluginwemo)

QT+= network

SOURCES += \
    devicepluginwemo.cpp \
    wemodiscovery.cpp \
    wemoswitch.cpp \
    wemoeventhandler.cpp


HEADERS += \
    devicepluginwemo.h \
    wemodiscovery.h \
    wemoswitch.h \
    wemoeventhandler.h



