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

#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include "plugin/deviceclass.h"
#include "plugin/device.h"
#include "plugin/devicedescriptor.h"

#include "types/event.h"
#include "types/action.h"
#include "types/vendor.h"

#include <QObject>
#include <QTimer>

class Device;
class DevicePlugin;
class Radio433;

class DeviceManager : public QObject
{
    Q_OBJECT
    Q_ENUMS(DeviceError)
public:
    enum HardwareResource {
        HardwareResourceNone = 0x00,
        HardwareResourceRadio433 = 0x01,
        HardwareResourceRadio868 = 0x02,
        HardwareResourceTimer = 0x04
    };
    Q_DECLARE_FLAGS(HardwareResources, HardwareResource)

    enum DeviceError {
        DeviceErrorNoError,
        DeviceErrorPluginNotFound,
        DeviceErrorDeviceNotFound,
        DeviceErrorDeviceClassNotFound,
        DeviceErrorActionTypeNotFound,
        DeviceErrorStateTypeNotFound,
        DeviceErrorEventTypeNotFound,
        DeviceErrorDeviceDescriptorNotFound,
        DeviceErrorMissingParameter,
        DeviceErrorInvalidParameter,
        DeviceErrorSetupFailed,
        DeviceErrorDuplicateUuid,
        DeviceErrorCreationMethodNotSupported,
        DeviceErrorSetupMethodNotSupported,
        DeviceErrorHardwareNotAvailable,
        DeviceErrorHardwareFailure,
        // TODO: Bump API version
        //DeviceErrorAuthentificationFailure,
        DeviceErrorAsync,
        DeviceErrorDeviceInUse,
        DeviceErrorPairingTransactionIdNotFound,
    };

    enum DeviceSetupStatus {
        DeviceSetupStatusSuccess,
        DeviceSetupStatusFailure,
        DeviceSetupStatusAsync
    };

    explicit DeviceManager(QObject *parent = 0);
    ~DeviceManager();

    QList<DevicePlugin*> plugins() const;
    DevicePlugin* plugin(const PluginId &id) const;
    DeviceError setPluginConfig(const PluginId &pluginId, const ParamList &pluginConfig);

    QList<Vendor> supportedVendors() const;
    QList<DeviceClass> supportedDevices(const VendorId &vendorId = VendorId()) const;
    DeviceError discoverDevices(const DeviceClassId &deviceClassId, const ParamList &params);

    QList<Device*> configuredDevices() const;
    DeviceError addConfiguredDevice(const DeviceClassId &deviceClassId, const ParamList &params, const DeviceId id = DeviceId::createDeviceId());
    DeviceError addConfiguredDevice(const DeviceClassId &deviceClassId, const DeviceDescriptorId &deviceDescriptorId, const DeviceId &id = DeviceId::createDeviceId());
    DeviceError pairDevice(const PairingTransactionId &pairingTransactionId, const DeviceClassId &deviceClassId, const ParamList &params);
    DeviceError pairDevice(const PairingTransactionId &pairingTransactionId, const DeviceClassId &deviceClassId, const DeviceDescriptorId &deviceDescriptorId);
    DeviceError confirmPairing(const PairingTransactionId &pairingTransactionId, const QString &secret = QString());
    DeviceError removeConfiguredDevice(const DeviceId &deviceId);

    Device* findConfiguredDevice(const DeviceId &id) const;
    QList<Device*> findConfiguredDevices(const DeviceClassId &deviceClassId) const;
    DeviceClass findDeviceClass(const DeviceClassId &deviceClassId) const;

signals:
    void loaded();
    void eventTriggered(const Event &event);
    void deviceStateChanged(Device *device, const QUuid &stateTypeId, const QVariant &value);
    void devicesDiscovered(const DeviceClassId &deviceClassId, const QList<DeviceDescriptor> &devices);
    void deviceSetupFinished(Device *device, DeviceError status);
    void pairingFinished(const PairingTransactionId &pairingTransactionId, DeviceError status, const DeviceId &deviceId = DeviceId());
    void actionExecutionFinished(const ActionId, DeviceError status);

public slots:
    DeviceError executeAction(const Action &action);

private slots:
    void loadPlugins();
    void loadConfiguredDevices();
    void storeConfiguredDevices();
    void startMonitoringAutoDevices();
    void slotDevicesDiscovered(const DeviceClassId &deviceClassId, const QList<DeviceDescriptor> deviceDescriptors);
    void slotDeviceSetupFinished(Device *device, DeviceManager::DeviceSetupStatus status);
    void slotPairingFinished(const PairingTransactionId &pairingTransactionId, DeviceManager::DeviceSetupStatus status);
    void autoDevicesAppeared(const DeviceClassId &deviceClassId, const QList<DeviceDescriptor> &deviceDescriptors);

    // Only connect this to Devices. It will query the sender()
    void slotDeviceStateValueChanged(const QUuid &stateTypeId, const QVariant &value);

    void radio433SignalReceived(QList<int> rawData);
    void timerEvent();

private:
    bool verifyPluginMetadata(const QJsonObject &data);
    DeviceError addConfiguredDeviceInternal(const DeviceClassId &deviceClassId, const ParamList &params, const DeviceId id = DeviceId::createDeviceId());
    DeviceSetupStatus setupDevice(Device *device);
    DeviceError verifyParams(const QList<ParamType> paramTypes, ParamList &params, bool requireAll = true);
    DeviceError verifyParam(const QList<ParamType> paramTypes, const Param &param);
    DeviceError verifyParam(const ParamType &paramType, const Param &param);

private:

    QHash<VendorId, Vendor> m_supportedVendors;
    QHash<VendorId, QList<DeviceClassId> > m_vendorDeviceMap;
    QHash<DeviceClassId, DeviceClass> m_supportedDevices;
    QList<Device*> m_configuredDevices;
    QHash<DeviceDescriptorId, DeviceDescriptor> m_discoveredDevices;

    QHash<PluginId, DevicePlugin*> m_devicePlugins;

    QString m_settingsFile;

    // Hardware Resources
    Radio433* m_radio433;
    QTimer m_pluginTimer;
    QList<Device*> m_pluginTimerUsers;

    QHash<QUuid, QPair<DeviceClassId, ParamList> > m_pairingsJustAdd;
    QHash<QUuid, QPair<DeviceClassId, DeviceDescriptorId> > m_pairingsDiscovery;

    QList<DevicePlugin*> m_discoveringPlugins;

    friend class DevicePlugin;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(DeviceManager::HardwareResources)
Q_DECLARE_METATYPE(DeviceManager::DeviceError)

#endif // DEVICEMANAGER_H
