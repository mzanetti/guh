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

DeviceClassId guhTuneDeviceClassId = DeviceClassId("4a20b247-576c-4a0f-bc95-9eb6b2f2eb3d");


DevicePluginGuhTune::DevicePluginGuhTune()
{
}

void DevicePluginGuhTune::startMonitoringAutoDevices()
{
    // make shore, we add only one guhTune device
    foreach (Device *device, myDevices()) {
        if (device->deviceClassId() == guhTuneDeviceClassId) {
            return;
        }
    }

    // check if we have a display
    if (qgetenv("DISPLAY").isEmpty()) {
        return;
    }

    // Hardware pushbutton
    m_button = new GuhButton(this, 18);
    if(m_button->isAvailable()){
        m_button->enable();
        connect(m_button, &GuhButton::pressed, this, &DevicePluginGuhTune::buttonPressed);
        connect(m_button, &GuhButton::released, this, &DevicePluginGuhTune::buttonReleased);
    }

    // timer for long pressed detection
    m_timer = new QTimer(this);
    m_timer->setInterval(500);
    m_timer->setSingleShot(true);
    connect(m_timer, &QTimer::timeout, this, &DevicePluginGuhTune::buttonLongPressed);

    // QML viewer
    QDeclarativeView *viewer = new QDeclarativeView();
    viewer->engine()->addImportPath(QLatin1String("modules"));
    viewer->engine()->rootContext()->setContextProperty("controller", this);
    viewer->setSource(QUrl(QLatin1String("qrc:///guhtune-ui/main.qml")));
    viewer->show();
}

DeviceManager::DeviceSetupStatus DevicePluginGuhTune::setupDevice(Device *device)
{

    Q_UNUSED(device);
    return DeviceManager::DeviceSetupStatusSuccess;
}

DeviceManager::HardwareResources DevicePluginGuhTune::requiredHardware() const
{
    return DeviceManager::HardwareResourceNone;
}

DeviceManager::DeviceError DevicePluginGuhTune::executeAction(Device *device, const Action &action)
{
    Q_UNUSED(device);
    Q_UNUSED(action);

    return DeviceManager::DeviceErrorNoError;
}

void DevicePluginGuhTune::invokeAction(int actionIndex, const QString &what)
{
    qDebug() << "button pressed" << actionIndex << what;
    // emit event here!
}

void DevicePluginGuhTune::buttonPressed()
{
    qDebug() << "button pressed";
    m_timer->start();
}

void DevicePluginGuhTune::buttonLongPressed()
{
    qDebug() << "button long pressed";
}

void DevicePluginGuhTune::buttonReleased()
{
    qDebug() << "button released";
    m_timer->stop();
}
