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

#ifndef DEVICEPLUGINELRO_H
#define DEVICEPLUGINELRO_H

#include "plugin/deviceplugin.h"
#include "guhbutton.h"

#include <QDebug>
#include <QTimer>

class DevicePluginGuhTune : public DevicePlugin
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "guru.guh.DevicePlugin" FILE "devicepluginguhtune.json")
    Q_INTERFACES(DevicePlugin)

public:
    explicit DevicePluginGuhTune();

    DeviceManager::HardwareResources requiredHardware() const override;
    void startMonitoringAutoDevices() override;
    DeviceManager::DeviceSetupStatus setupDevice(Device *device) override;

    Q_INVOKABLE void invokeAction(int actionIndex, const QString &what);

private:
    GuhButton *m_button;
    QTimer *m_timer;

private slots:
    void buttonPressed();
    void buttonLongPressed();
    void buttonReleased();

};

#endif // DEVICEPLUGINELRO_H
