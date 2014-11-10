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

#ifndef COLORANIMATION_H
#define COLORANIMATION_H

#include <QObject>
#include <QColor>
#include <QTimer>

#include "plugin/device.h"

class ColorAnimation : public QObject
{
    Q_OBJECT
public:
    explicit ColorAnimation(ActionId actionId = ActionId(), int channel = -1, QColor startColor = QColor(0,0,0), QColor endColor = QColor(255,255,255), int duration = 1000);
    void startAnimation();

    int channel();
    QColor startColor();
    QColor endColor();
    int duration();

private:
    ActionId m_actionId;
    int m_channel;
    QColor m_startColor;
    QColor m_endColor;
    int m_duration;

    QColor m_currentColor;
    QTimer *m_animationTimer;

signals:
    void updateColor(const int &channel, const QColor &color);
    void animationFinished(ActionId actionId);

private slots:
    void animate();
};

#endif // COLORANIMATION_H
