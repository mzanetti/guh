include(../guh.pri)

message("Building guh version $${GUH_VERSION_STRING}")

TARGET = guhd
TEMPLATE = app

INCLUDEPATH += ../libguh jsonrpc

target.path = /usr/bin
INSTALLS += target

QT += network declarative

LIBS += -L$$top_builddir/libguh/ -lguh

include(server.pri)
SOURCES += main.cpp

