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

#ifndef RULEENGINE_H
#define RULEENGINE_H

#include "rule.h"
#include "types/event.h"
#include "stateevaluator.h"

#include <QObject>
#include <QList>
#include <QUuid>

class RuleEngine : public QObject
{
    Q_OBJECT
    Q_ENUMS(RuleError)
    Q_ENUMS(RemovePolicy)
public:
    enum RuleError {
        RuleErrorNoError,
        RuleErrorInvalidRuleId,
        RuleErrorRuleNotFound,
        RuleErrorDeviceNotFound,
        RuleErrorEventTypeNotFound,
        RuleErrorActionTypeNotFound,
        RuleErrorInvalidParameter,
        RuleErrorMissingParameter
    };

    enum RemovePolicy {
        RemovePolicyCascade,
        RemovePolicyUpdate
    };

    explicit RuleEngine(QObject *parent = 0);

    QList<Action> evaluateEvent(const Event &event);

    RuleError addRule(const RuleId &ruleId, const QList<EventDescriptor> &eventDescriptorList, const QList<Action> &actions);
    RuleError addRule(const RuleId &ruleId, const QList<EventDescriptor> &eventDescriptorList, const StateEvaluator &stateEvaluator, const QList<Action> &actions);
    QList<Rule> rules() const;
    QList<RuleId> ruleIds() const;

    RuleError removeRule(const RuleId &ruleId);

    Rule findRule(const RuleId &ruleId);
    QList<RuleId> findRules(const DeviceId &deviceId);

    void removeDeviceFromRule(const RuleId &id, const DeviceId &deviceId);

signals:
    void ruleAdded(const RuleId &ruleId);
    void ruleRemoved(const RuleId &ruleId);

private:
    bool containsEvent(const Rule &rule, const Event &event);

    void appendRule(const Rule &rule);

private:
    QString m_settingsFile;
    QList<RuleId> m_ruleIds; // Keeping a list of RuleIds to keep sorting order...
    QHash<RuleId, Rule> m_rules; // ...but use a Hash for faster finding
};
Q_DECLARE_METATYPE(RuleEngine::RuleError)

#endif // RULEENGINE_H
