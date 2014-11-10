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

#include "devicepluginboblight.h"

#include "plugin/device.h"
#include "devicemanager.h"

#include "bobclient.h"

#include <QDebug>
#include <QStringList>

DeviceClassId boblightDeviceClassId = DeviceClassId("1647c61c-db14-461e-8060-8a3533d5d92f");
StateTypeId colorStateTypeId = StateTypeId("97ec80cd-43a9-40fa-93b7-d1580043d981");
StateTypeId brightnessStateTypeId = StateTypeId("f22a273b-cc27-4acf-b087-18d59380569f");
ActionTypeId setColorActionTypeId = ActionTypeId("668e1aa3-fa13-49ce-8630-17a5c0a7c34b");
ActionTypeId setPowerActionTypeId = ActionTypeId("5c4a0e04-c978-4666-8470-13ccba9436a1");
ActionTypeId setBrightnessActionTypeId = ActionTypeId("b8ee03a3-c63a-4a03-ad96-b549757fd1d7");

DevicePluginBoblight::DevicePluginBoblight()
{
    m_bobClient = new BobClient(this);
    connect(this, &DevicePlugin::configValueChanged, this, &DevicePluginBoblight::connectToBoblight);
}

DeviceManager::HardwareResources DevicePluginBoblight::requiredHardware() const
{
    return DeviceManager::HardwareResourceNone;
}

void DevicePluginBoblight::startMonitoringAutoDevices()
{
    if (!m_bobClient->connected()) {
        return;
    }

    QList<Device*> loadedDevices = deviceManager()->findConfiguredDevices(boblightDeviceClassId);

    QList<DeviceDescriptor> deviceDescriptorList;
    for (int i = loadedDevices.count(); i < m_bobClient->lightsCount(); i++) {
        DeviceDescriptor deviceDescriptor(boblightDeviceClassId, "Boblight (Channel " + QString::number(i) + ")");
        ParamList params;
        Param param("channel");
        param.setValue(i);
        params.append(param);
        deviceDescriptor.setParams(params);
        deviceDescriptorList.append(deviceDescriptor);
    }
    emit autoDevicesAppeared(boblightDeviceClassId, deviceDescriptorList);
}

DeviceManager::DeviceSetupStatus DevicePluginBoblight::setupDevice(Device *device)
{
    if (!m_bobClient->connected()) {
        return DeviceManager::DeviceSetupStatusFailure;
    }

    device->setName("Boblight (Channel " + device->paramValue("channel").toString() + ")");
    m_bobClient->currentColor(device->paramValue("channel").toInt());
    return DeviceManager::DeviceSetupStatusSuccess;
}

QList<ParamType> DevicePluginBoblight::configurationDescription() const
{
    QList<ParamType> params;
    params.append(ParamType("boblighthost", QVariant::String, "localhost"));
    params.append(ParamType("boblightport", QVariant::String, "19333"));
    return params;
}

DeviceManager::DeviceError DevicePluginBoblight::executeAction(Device *device, const Action &action)
{
    if (!m_bobClient->connected()) {
        return DeviceManager::DeviceErrorHardwareNotAvailable;
    }
    if (device->deviceClassId() == boblightDeviceClassId) {
        if(action.actionTypeId() == setColorActionTypeId) {
            QColor newColor = action.param("color").value().value<QColor>();
            qDebug() << "set to new color" << newColor;
            if (!newColor.isValid()) {
                return DeviceManager::DeviceErrorInvalidParameter;
            }

            if (action.params().paramValue("animation duration").toInt() == 0) {
                m_bobClient->setColor(device->paramValue("channel").toInt(), newColor);
                m_bobClient->sync();
                return DeviceManager::DeviceErrorNoError;
            }

            ColorAnimation *colorAnimation = new ColorAnimation(action.id(),
                                                                device->paramValue("channel").toInt(),
                                                                device->stateValue(colorStateTypeId).value<QColor>(),
                                                                newColor,
                                                                action.params().paramValue("animation duration").toInt());

            connect(colorAnimation, &ColorAnimation::updateColor, this, &DevicePluginBoblight::updateColor);
            connect(colorAnimation, &ColorAnimation::animationFinished, this, &DevicePluginBoblight::animationFinished);

            m_runningAnimations.insert(colorAnimation, device);

            colorAnimation->startAnimation();
            return DeviceManager::DeviceErrorAsync;
        }
        if (action.actionTypeId() == setPowerActionTypeId) {
            if (action.params().paramValue("power").toBool()) {
                ColorAnimation *colorAnimation = new ColorAnimation(action.id(),
                                                                    device->paramValue("channel").toInt(),
                                                                    device->stateValue(colorStateTypeId).value<QColor>(),
                                                                    QColor(255,255,255),
                                                                    500);

                connect(colorAnimation, &ColorAnimation::updateColor, this, &DevicePluginBoblight::updateColor);
                connect(colorAnimation, &ColorAnimation::animationFinished, this, &DevicePluginBoblight::animationFinished);

                m_runningAnimations.insert(colorAnimation, device);

                colorAnimation->startAnimation();
                return DeviceManager::DeviceErrorAsync;
            } else {
                ColorAnimation *colorAnimation = new ColorAnimation(action.id(),
                                                                    device->paramValue("channel").toInt(),
                                                                    device->stateValue(colorStateTypeId).value<QColor>(),
                                                                    QColor(0,0,0),
                                                                    500);

                connect(colorAnimation, &ColorAnimation::updateColor, this, &DevicePluginBoblight::updateColor);
                connect(colorAnimation, &ColorAnimation::animationFinished, this, &DevicePluginBoblight::animationFinished);

                m_runningAnimations.insert(colorAnimation, device);

                colorAnimation->startAnimation();
                return DeviceManager::DeviceErrorAsync;            }
            m_bobClient->sync();
        }
        if (action.actionTypeId() == setBrightnessActionTypeId) {
            m_bobClient->setBrightness(device->paramValue("channel").toInt(), action.params().paramValue("brightness").toInt());
            m_bobClient->sync();
        }
        return DeviceManager::DeviceErrorActionTypeNotFound;
    }
    return DeviceManager::DeviceErrorDeviceClassNotFound;

}

void DevicePluginBoblight::connectToBoblight()
{
    if (configValue("boblighthost").isValid() && configValue("boblightport").isValid()) {
        m_bobClient->connect(configValue("boblighthost").toString(), configValue("boblightport").toInt());
    }
}

void DevicePluginBoblight::updateColor(const int &channel, const QColor &newColor)
{
    qDebug() << "update color " << newColor;
    m_bobClient->setColor(channel, newColor);
    m_bobClient->sync();
}

void DevicePluginBoblight::animationFinished(ActionId actionId)
{
    ColorAnimation *colorAnimation = static_cast<ColorAnimation*>(sender());
    Device* device = m_runningAnimations.take(colorAnimation);
    device->setStateValue(colorStateTypeId, colorAnimation->endColor());
    emit actionExecutionFinished(actionId, DeviceManager::DeviceErrorNoError);
    colorAnimation->deleteLater();
}


