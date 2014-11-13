include(../../plugins.pri)

TARGET = $$qtLibraryTarget(guh_devicepluginguhtune)

QT += declarative quick

SOURCES += \
    devicepluginguhtune.cpp \
    guhbutton.cpp \
    guhencoder.cpp

HEADERS += \
    devicepluginguhtune.h \
    guhbutton.h \
    guhencoder.h

RESOURCES += qml.qrc \
            images.qrc

