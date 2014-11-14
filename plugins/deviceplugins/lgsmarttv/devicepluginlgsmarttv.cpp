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

#include "devicepluginlgsmarttv.h"

#include "plugin/device.h"
#include "devicemanager.h"

#include <QDebug>

DeviceClassId lgSmartTvDeviceClassId = DeviceClassId("1d41b5a8-74ff-4a12-b365-c7bbe610848f");

StateTypeId tvReachableStateTypeId = StateTypeId("b056c36b-df87-4177-8d5d-1e7c1e8cdc7a");
StateTypeId tv3DModeStateTypeId = StateTypeId("8ad3d77f-d340-495d-8c2a-5569a80e9d36");
StateTypeId tvVolumeLevelStateTypeId = StateTypeId("07d39a6e-7eab-42d0-851d-9f3bcd3bbb57");
StateTypeId tvMuteStateTypeId = StateTypeId("a6ac9061-3de7-403a-a646-790ca5d73764");
StateTypeId tvChannelTypeStateTypeId = StateTypeId("84c86670-77c7-4fc6-9e23-abca066e76aa");
StateTypeId tvChannelNameStateTypeId = StateTypeId("265dc5f7-3f4d-4002-a6fe-2a53986bcf1d");
StateTypeId tvChannelNumberStateTypeId = StateTypeId("881629a3-4ce2-42ba-8ce6-10d90c383799");
StateTypeId tvProgramNameStateTypeId = StateTypeId("3f53e52e-1ad7-40e7-8080-76908e720cac");
StateTypeId tvInputSourceIndexStateTypeId = StateTypeId("e895017a-139f-410c-bfb2-4d008104e164");
StateTypeId tvInputSourceLabelNameStateTypeId = StateTypeId("58b734ec-2269-4c57-99e1-e1eeee401053");


ActionTypeId commandVolumeUpActionTypeId = ActionTypeId("ac5d7dcd-dfe8-4a94-9ab9-21b3f804b39e");
ActionTypeId commandVolumeDownActionTypeId = ActionTypeId("62b17bec-f461-4ffa-93d1-67a9430d55e1");
ActionTypeId commandMuteActionTypeId = ActionTypeId("1aa9d7f0-0f66-4b90-bb72-f6b7b2118221");
ActionTypeId commandUnmuteActionTypeId = ActionTypeId("b7e31999-ba67-443d-8e5c-ec104af987bd");
ActionTypeId commandChannelUpActionTypeId = ActionTypeId("acd1f6a0-2cfa-4665-9607-cf94245ec5a3");
ActionTypeId commandChannelDownActionTypeId = ActionTypeId("6ea66772-0e6d-40b1-978c-a01fb53871dd");
ActionTypeId commandPowerOffActionTypeId = ActionTypeId("cbe41134-ff11-4916-815b-3ac289c64090");
ActionTypeId commandArrowUpActionTypeId = ActionTypeId("57c483b4-4ddf-4470-828c-8d8767e7a923");
ActionTypeId commandArrowDownActionTypeId = ActionTypeId("614cf1af-5cf7-4bb2-885c-4414078d8899");
ActionTypeId commandArrowLeftActionTypeId = ActionTypeId("916394dd-7833-4875-8d7a-49d7d24ceeb2");
ActionTypeId commandArrowRightActionTypeId = ActionTypeId("01e3df1e-638b-4e14-ba85-660267766062");
ActionTypeId commandOkActionTypeId = ActionTypeId("257dfa59-0d38-4e18-a3fc-213809fdb12f");
ActionTypeId commandBackActionTypeId = ActionTypeId("ce4184b3-6b8e-4fc3-a4cb-7b8ec72f2ce9");
ActionTypeId commandHomeActionTypeId = ActionTypeId("33f941c1-f5fc-4449-b6e3-93eafca493e0");
ActionTypeId commandInputSourceActionTypeId = ActionTypeId("9a6e5111-95d3-49ac-8056-249e704b1509");
ActionTypeId commandExitActionTypeId = ActionTypeId("d76efdb8-056e-4b39-a839-2ef6d6001b00");
ActionTypeId commandInfoActionTypeId = ActionTypeId("9c1290d5-3135-4124-a576-fc7522cffdcf");
ActionTypeId commandMyAppsActionTypeId = ActionTypeId("47d65cac-fe75-4c36-9dee-9862c1c1130e");
ActionTypeId commandProgramListActionTypeId = ActionTypeId("9aa3a97e-505d-4906-9764-14b6dc4e31e8");


DevicePluginLgSmartTv::DevicePluginLgSmartTv()
{
    //m_discovery = new TvDiscovery(this);

    //connect(m_discovery,SIGNAL(discoveryDone(QList<TvDevice*>)),this,SLOT(discoveryDone(QList<TvDevice*>)));
}

DeviceManager::DeviceError DevicePluginLgSmartTv::discoverDevices(const DeviceClassId &deviceClassId, const ParamList &params)
{
    qDebug() << "should discover devices with params:" << params;

    if(deviceClassId != lgSmartTvDeviceClassId){
        return DeviceManager::DeviceErrorDeviceClassNotFound;
    }

    m_discovery->discover(3000);

    return DeviceManager::DeviceErrorAsync;
}

DeviceManager::DeviceSetupStatus DevicePluginLgSmartTv::setupDevice(Device *device)
{

    device->setName("LG Smart Tv (" + device->paramValue("model").toString() + ")");

    TvDevice *tvDevice = new TvDevice(this);
    tvDevice->setName(device->paramValue("name").toString());
    tvDevice->setUuid(device->paramValue("uuid").toString());
    tvDevice->setModelName(device->paramValue("model").toString());
    tvDevice->setHostAddress(QHostAddress(device->paramValue("host address").toString()));
    tvDevice->setPort(device->paramValue("port").toInt());
    tvDevice->setLocation(QUrl(device->paramValue("location").toString()));
    tvDevice->setUuid(device->paramValue("manufacturer").toString());
    // key if there is one...

    tvDevice->setupEventHandler();

    connect(tvDevice, &TvDevice::pairingFinished, this, &DevicePluginLgSmartTv::pairingFinished);
    connect(tvDevice, &TvDevice::sendCommandFinished, this, &DevicePluginLgSmartTv::sendingCommandFinished);
    connect(tvDevice, &TvDevice::statusChanged, this, &DevicePluginLgSmartTv::statusChanged);

    tvDevice->requestPairing();
    m_tvList.insert(tvDevice,device);

    return DeviceManager::DeviceSetupStatusAsync;
}

DeviceManager::HardwareResources DevicePluginLgSmartTv::requiredHardware() const
{
    return DeviceManager::HardwareResourceTimer;
}

DeviceManager::DeviceError DevicePluginLgSmartTv::executeAction(Device *device, const Action &action)
{
    TvDevice * tvDevice = m_tvList.key(device);

    if(action.actionTypeId() == commandVolumeUpActionTypeId){
        tvDevice->sendCommand(TvDevice::VolUp, action.id());
    } else if(action.actionTypeId() == commandVolumeDownActionTypeId){
        tvDevice->sendCommand(TvDevice::VolDown, action.id());
    } else if(action.actionTypeId() == commandMuteActionTypeId){
        tvDevice->sendCommand(TvDevice::Mute, action.id());
    } else if(action.actionTypeId() == commandChannelUpActionTypeId){
        tvDevice->sendCommand(TvDevice::ChannelUp, action.id());
    } else if(action.actionTypeId() == commandChannelDownActionTypeId){
        tvDevice->sendCommand(TvDevice::ChannelDown, action.id());
    } else if(action.actionTypeId() == commandPowerOffActionTypeId){
        tvDevice->sendCommand(TvDevice::Power, action.id());
    } else if(action.actionTypeId() == commandArrowUpActionTypeId){
        tvDevice->sendCommand(TvDevice::Up, action.id());
    } else if(action.actionTypeId() == commandArrowDownActionTypeId){
        tvDevice->sendCommand(TvDevice::Down, action.id());
    } else if(action.actionTypeId() == commandArrowLeftActionTypeId){
        tvDevice->sendCommand(TvDevice::Left, action.id());
    } else if(action.actionTypeId() == commandArrowRightActionTypeId){
        tvDevice->sendCommand(TvDevice::Right, action.id());
    } else if(action.actionTypeId() == commandOkActionTypeId){
        tvDevice->sendCommand(TvDevice::Ok, action.id());
    } else if(action.actionTypeId() == commandBackActionTypeId){
        tvDevice->sendCommand(TvDevice::Back, action.id());
    } else if(action.actionTypeId() == commandHomeActionTypeId){
        tvDevice->sendCommand(TvDevice::Home, action.id());
    } else if(action.actionTypeId() == commandInputSourceActionTypeId){
        tvDevice->sendCommand(TvDevice::ExternalInput, action.id());
    } else if(action.actionTypeId() == commandExitActionTypeId){
        tvDevice->sendCommand(TvDevice::Exit, action.id());
    } else if(action.actionTypeId() == commandInfoActionTypeId){
        tvDevice->sendCommand(TvDevice::Info, action.id());
    } else if(action.actionTypeId() == commandMyAppsActionTypeId){
        tvDevice->sendCommand(TvDevice::MyApps, action.id());
    } else if(action.actionTypeId() == commandProgramListActionTypeId){
        tvDevice->sendCommand(TvDevice::ProgramList, action.id());
    } else {
        return DeviceManager::DeviceErrorActionTypeNotFound;
    }
    return DeviceManager::DeviceErrorAsync;
}

void DevicePluginLgSmartTv::deviceRemoved(Device *device)
{
    if (!m_tvList.values().contains(device)) {
        return;
    }

    TvDevice *tvDevice= m_tvList.key(device);
    qDebug() << "remove LG Smart Tv  " << tvDevice->modelName();
    m_tvList.remove(tvDevice);
    tvDevice->deleteLater();
}

void DevicePluginLgSmartTv::guhTimer()
{
    foreach (TvDevice *tvDevice, m_tvList.keys()) {
        tvDevice->refresh();
    }
}

void DevicePluginLgSmartTv::discoveryDone(QList<TvDevice*> tvList)
{
    QList<DeviceDescriptor> deviceDescriptors;
    foreach (TvDevice *device, tvList) {
        DeviceDescriptor descriptor(lgSmartTvDeviceClassId, "Lg Smart Tv", device->modelName());
        ParamList params;
        params.append(Param("name", device->name()));
        params.append(Param("uuid", device->uuid()));
        params.append(Param("model", device->modelName()));
        params.append(Param("host address", device->hostAddress().toString()));
        params.append(Param("location", device->location().toString()));
        params.append(Param("port", device->port()));
        params.append(Param("manufacturer", device->manufacturer()));
        params.append(Param("key", device->key()));
        descriptor.setParams(params);
        deviceDescriptors.append(descriptor);
    }
    emit devicesDiscovered(lgSmartTvDeviceClassId, deviceDescriptors);
}

void DevicePluginLgSmartTv::pairingFinished(const bool &success)
{
    TvDevice *tvDevice = static_cast<TvDevice*>(sender());
    Device *device = m_tvList.value(tvDevice);

    if(success){
        emit deviceSetupFinished(device,DeviceManager::DeviceSetupStatusSuccess);
        tvDevice->refresh();
    }else{
        emit deviceSetupFinished(device,DeviceManager::DeviceSetupStatusFailure);
    }
}

void DevicePluginLgSmartTv::sendingCommandFinished(const bool &success, const ActionId &actionId)
{
    if(success){
        emit actionExecutionFinished(actionId,DeviceManager::DeviceErrorNoError);
    }else{
        emit actionExecutionFinished(actionId,DeviceManager::DeviceErrorHardwareFailure);
    }
}

void DevicePluginLgSmartTv::statusChanged()
{
    TvDevice *tvDevice = static_cast<TvDevice*>(sender());
    Device *device = m_tvList.value(tvDevice);

    device->setStateValue(tvReachableStateTypeId, tvDevice->reachable());
    device->setStateValue(tv3DModeStateTypeId, tvDevice->is3DMode());
    device->setStateValue(tvVolumeLevelStateTypeId, tvDevice->volumeLevel());
    device->setStateValue(tvMuteStateTypeId, tvDevice->mute());
    device->setStateValue(tvChannelTypeStateTypeId, tvDevice->channelType());
    device->setStateValue(tvChannelNameStateTypeId, tvDevice->channelName());
    device->setStateValue(tvChannelNumberStateTypeId, tvDevice->channelNumber());
    device->setStateValue(tvProgramNameStateTypeId, tvDevice->programName());
    device->setStateValue(tvInputSourceIndexStateTypeId, tvDevice->inputSourceIndex());
    device->setStateValue(tvInputSourceLabelNameStateTypeId, tvDevice->inputSourceLabelName());
}


