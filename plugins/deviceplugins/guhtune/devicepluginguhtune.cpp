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

#include <QDebug>
#include <QStringList>
#include <QDeclarativeView>
#include <QDeclarativeComponent>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>

DeviceClassId guhTuneItemDeviceClassId = DeviceClassId("4a20b247-576c-4a0f-bc95-9eb6b2f2eb3d");
StateTypeId selectedStateTypeId = StateTypeId("191262bc-de9e-44e0-b88f-e5b9c25e10e0");
EventTypeId clockwiseTickEventTypeId = EventTypeId("eeed2786-02eb-4a57-8e61-149741afb58c");
EventTypeId counterClockwiseTickEventTypeId = EventTypeId("1bc20ff4-4a53-4346-9477-c2c15fc11d61");
EventTypeId pressedEventTypeId = EventTypeId("e6fc4dce-3ce8-4651-a966-4c259e3c2a7a");
EventTypeId releasedEventTypeId = EventTypeId("9d26638d-f816-464a-adbc-38e8a2847914");

DevicePluginGuhTune::DevicePluginGuhTune()
{
}

void DevicePluginGuhTune::startMonitoringAutoDevices()
{
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
    for (int i = 0; i < 4; ++i) {
        DeviceDescriptor deviceDescriptor(guhTuneItemDeviceClassId, "guhTune item (" + QString::number(i) + ")");
        ParamList params;
        Param param("item");
        param.setValue(i);
        params.append(param);
        deviceDescriptor.setParams(params);
        deviceDescriptorList.append(deviceDescriptor);
    }
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
            connect(m_button, &GuhButton::buttonLongPressed, this, &DevicePluginGuhTune::buttonLongPressed);
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
            connect(m_encoder, &GuhEncoder::buttonLongPressed, this, &DevicePluginGuhTune::buttonLongPressed);
        } else {
            qDebug() << " ----> ERROR: hardware button NOT found.";
            m_encoder->deleteLater();
        }

        // QML viewer
        QDeclarativeView *viewer = new QDeclarativeView();
        viewer->engine()->addImportPath(QLatin1String("modules"));
        viewer->engine()->rootContext()->setContextProperty("controller", this);
        viewer->setSource(QUrl(QLatin1String("qrc:///guhtune-ui/main.qml")));
        viewer->show();
    }

    device->setName("guhTune item (" + device->paramValue("item").toString() + ")");

    Q_UNUSED(device);
    return DeviceManager::DeviceSetupStatusSuccess;
}

DeviceManager::HardwareResources DevicePluginGuhTune::requiredHardware() const
{
    return DeviceManager::HardwareResourceNone;
}

void DevicePluginGuhTune::invokeAction(int actionIndex, const QString &what)
{

    foreach (Device* device, myDevices()) {
        if (device->paramValue("item").toInt() == actionIndex) {
            if (what == "pressed") {
                emit emitEvent(Event(pressedEventTypeId, device->id()));
            } else if (what == "released") {
                emit emitEvent(Event(releasedEventTypeId, device->id()));
            } else if (what == "increase") {
                emit emitEvent(Event(clockwiseTickEventTypeId, device->id()));
            } else if (what == "decrease") {
                emit emitEvent(Event(counterClockwiseTickEventTypeId, device->id()));
            } else {
                qDebug() << "unknown action " << what;
            }
        }
    }
}

void DevicePluginGuhTune::buttonPressed()
{
    qDebug() << "button pressed";
}

void DevicePluginGuhTune::buttonLongPressed()
{
    qDebug() << "button long pressed";
}

void DevicePluginGuhTune::buttonReleased()
{
    qDebug() << "button released";
}

void DevicePluginGuhTune::encoderIncreased()
{
    qDebug() << "encoder  +";
}

void DevicePluginGuhTune::encoderDecreased()
{
    qDebug() << "encoder  -";
}
