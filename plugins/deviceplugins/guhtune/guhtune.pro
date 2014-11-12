include(../../plugins.pri)

TARGET = $$qtLibraryTarget(guh_devicepluginguhtune)

QT += declarative quick

SOURCES += \
    devicepluginguhtune.cpp \
    guhbutton.cpp

HEADERS += \
    devicepluginguhtune.h \
    guhbutton.h

RESOURCES += qml.qrc \
            images.qrc

