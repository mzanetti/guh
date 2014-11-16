include(../../plugins.pri)

TARGET = $$qtLibraryTarget(guh_devicepluginguhtune)

QT += declarative quick

SOURCES += \
    devicepluginguhtune.cpp \
    guhbutton.cpp \
    guhencoder.cpp \
    tuneui.cpp

HEADERS += \
    devicepluginguhtune.h \
    guhbutton.h \
    guhencoder.h \
    tuneui.h

RESOURCES += qml.qrc \
            images.qrc

