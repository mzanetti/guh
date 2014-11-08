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

#ifndef DEVICEPLUGINGENERICELEMENTS_H
#define DEVICEPLUGINGENERICELEMENTS_H

#include "plugin/deviceplugin.h"

class DevicePluginGenericElements : public DevicePlugin
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "guru.guh.DevicePlugin" FILE "deviceplugingenericelements.json")
    Q_INTERFACES(DevicePlugin)

public:
    explicit DevicePluginGenericElements();

    DeviceManager::HardwareResources requiredHardware() const override;
    DeviceManager::DeviceSetupStatus setupDevice(Device *device) override;


public slots:
    DeviceManager::DeviceError executeAction(Device *device, const Action &action) override;

};

#endif // DEVICEPLUGINGENERICELEMENTS_H
