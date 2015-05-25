include(../guh.pri)

TARGET = guhd
TEMPLATE = app

INCLUDEPATH += ../libguh jsonrpc

!ubuntu {
    target.path = /usr/bin
}
ubuntu {
    load(ubuntu-click)
    target.path = $${UBUNTU_CLICK_BINARY_PATH}
}
INSTALLS += target

QT += network sql
LIBS += -L$$top_builddir/libguh/ -lguh


include(server.pri)
include(qtservice/qtservice.pri)

SOURCES += main.cpp \
    guhservice.cpp

boblight {
    xcompile {
        LIBS += -L../plugins/deviceplugins/boblight -lguh_devicepluginboblight -lboblight
    } else {
        LIBS += -L../plugins/deviceplugins/boblight -lguh_devicepluginboblight -L/usr/local/lib/ -lboblight
    }
    DEFINES += USE_BOBLIGHT
}

HEADERS += \
    guhservice.h
