/****************************************************************************
 *                                                                          *
 *  This file is part of guh.                                               *
 *                                                                          *
 *  Guh is free software: you can redistribute it and/or modify             *
 *  it under the terms of the GNU General Public License as published by    *
 *  the Free Software Foundation, version 2 of the License.                 *
 *                                                                          *
 *  Guh is distributed in the hope that it will be useful,                  *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *  GNU General Public License for more details.                            *
 *                                                                          *
 *  You should have received a copy of the GNU General Public License       *
 *  along with guh.  If not, see <http://www.gnu.org/licenses/>.            *
 *                                                                          *
 ***************************************************************************/

#ifndef DEVICEPLUGINWEATHERGROUND_H
#define DEVICEPLUGINWEATHERGROUND_H

#include "plugin/deviceplugin.h"
#include "weathergroundparser.h"


class DevicePluginWeatherground : public DevicePlugin
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "org.guh.DevicePlugin" FILE "devicepluginweatherground.json")
    Q_INTERFACES(DevicePlugin)

public:
    explicit DevicePluginWeatherground();

    WeathergroundParser *m_parser;

    QList<Vendor> supportedVendors() const override;
    QList<DeviceClass> supportedDevices() const override;
    DeviceManager::HardwareResources requiredHardware() const override;

    QString pluginName() const override;
    QUuid pluginId() const override;

    void guhTimer() override;

private slots:

public slots:


};

#endif // DEVICEPLUGINWEATHERGROUND_H
