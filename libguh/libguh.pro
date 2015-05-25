include(../guh.pri)

TARGET = guh
TEMPLATE = lib

QT += network

!ubuntu {
    target.path = /usr/lib
}
ubuntu {
    load(ubuntu-click)
    target.path = $${UBUNTU_CLICK_BINARY_PATH}
}
INSTALLS += target

SOURCES += plugin/device.cpp \
           plugin/deviceclass.cpp \
           plugin/deviceplugin.cpp \
           plugin/devicedescriptor.cpp \
           devicemanager.cpp \
           hardware/gpio.cpp \
           hardware/gpiomonitor.cpp \
           hardware/radio433/radio433.cpp \
           hardware/radio433/radio433transmitter.cpp \
           hardware/radio433/radio433receiver.cpp \
           hardware/radio433/radio433brennenstuhlgateway.cpp \
           network/upnpdiscovery/upnpdiscovery.cpp \
           network/upnpdiscovery/upnpdevice.cpp \
           network/upnpdiscovery/upnpdevicedescriptor.cpp \
           network/upnpdiscovery/upnpdiscoveryrequest.cpp \
           network/networkmanager.cpp \
           types/action.cpp \
           types/actiontype.cpp \
           types/state.cpp \
           types/statetype.cpp \
           types/eventtype.cpp \
           types/event.cpp \
           types/eventdescriptor.cpp \
           types/vendor.cpp \
           types/paramtype.cpp \
           types/param.cpp \
           types/paramdescriptor.cpp \
           types/ruleaction.cpp \
           types/ruleactionparam.cpp \
           types/statedescriptor.cpp \

HEADERS += plugin/device.h \
           plugin/deviceclass.h \
           plugin/deviceplugin.h \
           plugin/devicedescriptor.h \
           devicemanager.h \
           hardware/gpio.h \
           hardware/gpiomonitor.h \
           hardware/radio433/radio433.h \
           hardware/radio433/radio433transmitter.h \
           hardware/radio433/radio433receiver.h \
           hardware/radio433/radio433brennenstuhlgateway.h \
           network/upnpdiscovery/upnpdiscovery.h \
           network/upnpdiscovery/upnpdevice.h \
           network/upnpdiscovery/upnpdevicedescriptor.h \
           network/upnpdiscovery/upnpdiscoveryrequest.h \
           network/networkmanager.h \
           types/action.h \
           types/actiontype.h \
           types/state.h \
           types/statetype.h \
           types/eventtype.h \
           types/event.h \
           types/eventdescriptor.h \
           types/vendor.h \
           types/typeutils.h \
           types/paramtype.h \
           types/param.h \
           types/paramdescriptor.h \
           types/ruleaction.h \
           types/ruleactionparam.h \
           types/statedescriptor.h \
           typeutils.h \

