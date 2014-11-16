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

#include "devicepluginguhtune.h"
#include "devicemanager.h"
#include "guhbutton.h"
#include "guhencoder.h"
#include "tuneui.h"

#include <QDebug>
#include <QStringList>

DeviceClassId guhTuneItemDeviceClassId = DeviceClassId("4a20b247-576c-4a0f-bc95-9eb6b2f2eb3d");
EventTypeId selectedEventTypeId = EventTypeId("eeed2786-02eb-4a57-8e61-149741afb58c");
EventTypeId setValueEventTypeId = EventTypeId("1bc20ff4-4a53-4346-9477-c2c15fc11d61");

DevicePluginGuhTune::DevicePluginGuhTune()
{
}

void DevicePluginGuhTune::startMonitoringAutoDevices()
{
    qDebug() << "*****************************************";
    // make shore, we add only one guhTune device
    foreach (Device *device, myDevices()) {
        if (device->deviceClassId() == guhTuneItemDeviceClassId) {
            return;
        }
    }

    // check if we have a display
    if (qgetenv("DISPLAY").isEmpty()) {
        qDebug() << " ----> ERROR: no display found";
        return;
    }
    qDebug() << " ----> display found";

    QList<DeviceDescriptor> deviceDescriptorList;
    DeviceDescriptor deviceDescriptor(guhTuneItemDeviceClassId, "guhTune device");
    deviceDescriptorList.append(deviceDescriptor);
    emit autoDevicesAppeared(guhTuneItemDeviceClassId, deviceDescriptorList);
}

DeviceManager::DeviceSetupStatus DevicePluginGuhTune::setupDevice(Device *device)
{

    if (myDevices().isEmpty()) {

        // Hardware pushbutton
        m_button = new GuhButton(this, 14);
        if(m_button->enable()){
            qDebug() << " ----> hardware button found.";
            connect(m_button, &GuhButton::buttonPressed, this, &DevicePluginGuhTune::buttonPressed);
            connect(m_button, &GuhButton::buttonReleased, this, &DevicePluginGuhTune::buttonReleased);
        } else {
            qDebug() << " ----> ERROR: hardware button NOT found.";
            m_button->deleteLater();
        }

        // Hardware encoder
        m_encoder = new GuhEncoder(this, 4, 2, 3);
        if(m_encoder->enable()){
            qDebug() << " ----> hardware encoder found.";
            connect(m_encoder, &GuhEncoder::increased, this, &DevicePluginGuhTune::encoderIncreased);
            connect(m_encoder, &GuhEncoder::decreased, this, &DevicePluginGuhTune::encoderDecreased);
            connect(m_encoder, &GuhEncoder::buttonPressed, this, &DevicePluginGuhTune::buttonPressed);
            connect(m_encoder, &GuhEncoder::buttonReleased, this, &DevicePluginGuhTune::buttonReleased);
        } else {
            qDebug() << " ----> ERROR: hardware button NOT found.";
            m_encoder->deleteLater();
        }

        m_ui = new TuneUi(this);
    }

    device->setName("guhTune item (" + device->paramValue("item").toString() + ")");

    Q_UNUSED(device);
    return DeviceManager::DeviceSetupStatusSuccess;
}

DeviceManager::HardwareResources DevicePluginGuhTune::requiredHardware() const
{
    return DeviceManager::HardwareResourceNone;
}

void DevicePluginGuhTune::buttonPressed()
{
    m_ui->buttonPressed();
}

void DevicePluginGuhTune::buttonReleased()
{
    m_ui->buttonReleased();
}

void DevicePluginGuhTune::encoderIncreased()
{
    m_ui->smallStep(TuneUi::RotationRight);
}

void DevicePluginGuhTune::encoderDecreased()
{
    m_ui->smallStep(TuneUi::RotationLeft);
}

// TODO: bigStep


void DevicePluginGuhTune::pressed(int actionIndex)
{
    qDebug() << actionIndex;
}

void DevicePluginGuhTune::increase(int actionIndex)
{
    qDebug() << actionIndex;
}

void DevicePluginGuhTune::decrease(int actionIndex)
{
    qDebug() << actionIndex;
}
