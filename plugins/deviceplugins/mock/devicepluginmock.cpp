/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  This file is part of guh.                                              *
 *                                                                         *
 *  Guh is free software: you can redistribute it and/or modify            *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, version 2 of the License.                *
 *                                                                         *
 *  Guh is distributed in the hope that it will be useful,                 *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with guh. If not, see <http://www.gnu.org/licenses/>.            *
 *                                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "devicepluginmock.h"
#include "httpdaemon.h"

#include "plugin/device.h"
#include "devicemanager.h"

#include <QDebug>
#include <QStringList>

DeviceClassId mockDeviceClassId = DeviceClassId("753f0d32-0468-4d08-82ed-1964aab03298");
DeviceClassId mockDeviceAutoClassId = DeviceClassId("ab4257b3-7548-47ee-9bd4-7dc3004fd197");
//DeviceClassId mockDeviceDiscoveryClassId = DeviceClassId("1bbaf751-36b7-4d3d-b05a-58dab2a3be8c");
//DeviceClassId mockDeviceAsyncSetupClassId = DeviceClassId("c08a8b27-8200-413d-b96b-4cff78b864d9");
//DeviceClassId mockDeviceBrokenClassId = DeviceClassId("ba5fb404-c9ce-4db4-8cd4-f48c61c24b13");
//DeviceClassId mockDeviceBrokenAsyncSetupClassId = DeviceClassId("bd5b78c5-53c9-4417-8eac-8ab2bce97bd0");
EventTypeId mockEvent1Id = EventTypeId("45bf3752-0fc6-46b9-89fd-ffd878b5b22b");
EventTypeId mockEvent2Id = EventTypeId("863d5920-b1cf-4eb9-88bd-8f7b8583b1cf");
StateTypeId mockIntStateId = StateTypeId("80baec19-54de-4948-ac46-31eabfaceb83");
StateTypeId mockBoolStateId = StateTypeId("9dd6a97c-dfd1-43dc-acbd-367932742310");
ActionTypeId mockActionIdWithParams = ActionTypeId("dea0f4e1-65e3-4981-8eaa-2701c53a9185");
ActionTypeId mockActionIdNoParams = ActionTypeId("defd3ed6-1a0d-400b-8879-a0202cf39935");
ActionTypeId mockActionIdAsync = ActionTypeId("fbae06d3-7666-483e-a39e-ec50fe89054e");
ActionTypeId mockActionIdFailing = ActionTypeId("df3cf33d-26d5-4577-9132-9823bd33fad0");
ActionTypeId mockActionIdAsyncFailing = ActionTypeId("bfe89a1d-3497-4121-8318-e77c37537219");

DevicePluginMock::DevicePluginMock()
{
}

DevicePluginMock::~DevicePluginMock()
{
}

DeviceManager::HardwareResources DevicePluginMock::requiredHardware() const
{
    return DeviceManager::HardwareResourceTimer;
}

DeviceManager::DeviceError DevicePluginMock::discoverDevices(const DeviceClassId &deviceClassId, const ParamList &params)
{
    Q_UNUSED(deviceClassId)
    qDebug() << "starting mock discovery:" << params;
    m_discoveredDeviceCount = params.paramValue("resultCount").toInt();
    QTimer::singleShot(1000, this, SLOT(emitDevicesDiscovered()));
    return DeviceManager::DeviceErrorAsync;
}

DeviceManager::DeviceSetupStatus DevicePluginMock::setupDevice(Device *device)
{
    qDebug() << "Mockdevice created returning true" << device->paramValue("httpport").toInt() << device->paramValue("async").toBool() << device->paramValue("broken").toBool();

    if (device->paramValue("broken").toBool()) {
        qWarning() << "This device is intentionally broken.";
        return DeviceManager::DeviceSetupStatusFailure;
    }

    HttpDaemon *daemon = new HttpDaemon(device, this);
    m_daemons.insert(device, daemon);

    if (!daemon->isListening()) {
        qWarning() << "HTTP port opening failed.";
        return DeviceManager::DeviceSetupStatusFailure;
    }

    connect(daemon, &HttpDaemon::triggerEvent, this, &DevicePluginMock::triggerEvent);
    connect(daemon, &HttpDaemon::setState, this, &DevicePluginMock::setState);

    if (device->paramValue("async").toBool()) {
        m_asyncSetupDevices.append(device);
        QTimer::singleShot(1000, this, SLOT(emitDeviceSetupFinished()));
        return DeviceManager::DeviceSetupStatusAsync;
    }
    return DeviceManager::DeviceSetupStatusSuccess;
}

void DevicePluginMock::deviceRemoved(Device *device)
{
    delete m_daemons.take(device);
}

void DevicePluginMock::startMonitoringAutoDevices()
{
    foreach (Device *device, myDevices()) {
        if (device->deviceClassId() == mockDeviceAutoClassId) {
            return; // We already have a Auto Mock device... do nothing.
        }
    }

    DeviceDescriptor mockDescriptor(mockDeviceAutoClassId, "Mock Device (Auto created)");

    ParamList params;
    qsrand(QDateTime::currentMSecsSinceEpoch());
    int port = 4242 + (qrand() % 1000);
    Param param("httpport", port);
    params.append(param);
    mockDescriptor.setParams(params);

    QList<DeviceDescriptor> deviceDescriptorList;
    deviceDescriptorList.append(mockDescriptor);

    emit autoDevicesAppeared(mockDeviceAutoClassId, deviceDescriptorList);
}

QList<ParamType> DevicePluginMock::configurationDescription() const
{
    QList<ParamType> params;
    ParamType mockParam1("configParamInt", QVariant::Int, 42);
    mockParam1.setLimits(1, 50);
    params.append(mockParam1);

    ParamType mockParam2("configParamBool", QVariant::Bool, true);
    params.append(mockParam2);

    return params;
}

DeviceManager::DeviceError DevicePluginMock::executeAction(Device *device, const Action &action)
{
    if (!myDevices().contains(device)) {
        return DeviceManager::DeviceErrorDeviceNotFound;
    }

    if (action.actionTypeId() == mockActionIdAsync || action.actionTypeId() == mockActionIdAsyncFailing) {
        m_asyncActions.append(qMakePair<Action, Device*>(action, device));
        QTimer::singleShot(1000, this, SLOT(emitActionExecuted()));
        return DeviceManager::DeviceErrorAsync;
    }

    if (action.actionTypeId() == mockActionIdFailing) {
        return DeviceManager::DeviceErrorSetupFailed;
    }

    m_daemons.value(device)->actionExecuted(action.actionTypeId());
    return DeviceManager::DeviceErrorNoError;
}

void DevicePluginMock::setState(const StateTypeId &stateTypeId, const QVariant &value)
{
    HttpDaemon *daemon = qobject_cast<HttpDaemon*>(sender());
    if (!daemon) {
        return;
    }

    Device *device = m_daemons.key(daemon);
    device->setStateValue(stateTypeId, value);
}

void DevicePluginMock::triggerEvent(const EventTypeId &id)
{
    HttpDaemon *daemon = qobject_cast<HttpDaemon*>(sender());
    if (!daemon) {
        return;
    }

    Device *device = m_daemons.key(daemon);

    Event event(id, device->id());

    qDebug() << "Emitting event " << event.eventTypeId();
    emit emitEvent(event);
}

void DevicePluginMock::emitDevicesDiscovered()
{
    QList<DeviceDescriptor> deviceDescriptors;

    if (m_discoveredDeviceCount > 0) {
        DeviceDescriptor d1(mockDeviceClassId, "Mock Device (Discovered)");
        ParamList params;
        Param httpParam("httpport", "55555");
        params.append(httpParam);
        d1.setParams(params);
        deviceDescriptors.append(d1);
    }

    if (m_discoveredDeviceCount > 1) {
        DeviceDescriptor d2(mockDeviceClassId, "Mock Device (Discovered)");
        ParamList params;
        Param httpParam("httpport", "55556");
        params.append(httpParam);
        d2.setParams(params);
        deviceDescriptors.append(d2);
    }

    emit devicesDiscovered(mockDeviceClassId, deviceDescriptors);
}

void DevicePluginMock::emitDeviceSetupFinished()
{
    qDebug() << "emitting setup finised";
    Device *device = m_asyncSetupDevices.takeFirst();
    if (device->paramValue("broken").toBool()) {
        emit deviceSetupFinished(device, DeviceManager::DeviceSetupStatusFailure);
    } else {
        emit deviceSetupFinished(device, DeviceManager::DeviceSetupStatusSuccess);
    }
}

void DevicePluginMock::emitActionExecuted()
{
    QPair<Action, Device*> action = m_asyncActions.takeFirst();
    if (action.first.actionTypeId() == mockActionIdAsync) {
        m_daemons.value(action.second)->actionExecuted(action.first.actionTypeId());
        emit actionExecutionFinished(action.first.id(), DeviceManager::DeviceErrorNoError);
    } else if (action.first.actionTypeId() == mockActionIdAsyncFailing) {
        emit actionExecutionFinished(action.first.id(), DeviceManager::DeviceErrorSetupFailed);
    }
}
