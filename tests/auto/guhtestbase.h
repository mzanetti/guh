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

#ifndef GUHTESTBASE_H
#define GUHTESTBASE_H

#include "typeutils.h"
#include "mocktcpserver.h"
#include "devicemanager.h"
#include "ruleengine.h"
#include "jsontypes.h"

#include <QObject>
#include <QUuid>
#include <QVariantMap>
#include <QtTest>

extern DeviceClassId mockDeviceClassId;
extern DeviceClassId mockDeviceAutoClassId;
extern DeviceClassId mockDeviceDiscoveryClassId;
extern DeviceClassId mockDeviceAsyncSetupClassId;
extern DeviceClassId mockDeviceBrokenClassId;
extern DeviceClassId mockDeviceBrokenAsyncSetupClassId;
extern ActionTypeId mockActionIdWithParams;
extern ActionTypeId mockActionIdNoParams;
extern ActionTypeId mockActionIdAsync;
extern ActionTypeId mockActionIdFailing;
extern ActionTypeId mockActionIdAsyncFailing;
extern EventTypeId mockEvent1Id;
extern EventTypeId mockEvent2Id;
extern StateTypeId mockIntStateId;
extern StateTypeId mockBoolStateId;

class MockTcpServer;

class GuhTestBase : public QObject
{
    Q_OBJECT
public:
    explicit GuhTestBase(QObject *parent = 0);

protected slots:
    void initTestCase();
    void cleanupTestCase();

protected:
    QVariant injectAndWait(const QString &method, const QVariantMap &params = QVariantMap());

    inline void verifyError(const QVariant &response, const QString &fieldName, const QString &error)
    {
        QJsonDocument jsonDoc = QJsonDocument::fromVariant(response);
        QVERIFY2(response.toMap().value("status").toString() == QString("success"), jsonDoc.toJson().data());
        QVERIFY2(response.toMap().value("params").toMap().value(fieldName).toString() == error,
                 QString("\nExpected: %1\nGot: %2\nFull message: %3\n")
                 .arg(error)
                 .arg(response.toMap().value("params").toMap().value(fieldName).toString())
                 .arg(jsonDoc.toJson().data())
                 .toLatin1().data());
    }

    inline void verifyRuleError(const QVariant &response, RuleEngine::RuleError error = RuleEngine::RuleErrorNoError) {
        verifyError(response, "ruleError", JsonTypes::ruleErrorToString(error));
    }

    inline void verifyDeviceError(const QVariant &response, DeviceManager::DeviceError error = DeviceManager::DeviceErrorNoError) {
        verifyError(response, "deviceError", JsonTypes::deviceErrorToString(error));
    }

    void restartServer();

protected:
    PluginId mockPluginId = PluginId("727a4a9a-c187-446f-aadf-f1b2220607d1");
    VendorId guhVendorId = VendorId("2062d64d-3232-433c-88bc-0d33c0ba2ba6");

    MockTcpServer *m_mockTcpServer;
    QUuid m_clientId;
    int m_commandId;

    int m_mockDevice1Port;
    int m_mockDevice2Port;

    DeviceId m_mockDeviceId;

    QString m_deviceSettings;
    QString m_rulesSettings;
};

#endif // GUHTESTBASE_H
