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

#ifndef GUHBUTTON_H
#define GUHBUTTON_H

#include <QObject>
#include <QDebug>
#include <QTimer>

#include "hardware/gpiomonitor.h"

class GuhButton : public QObject
{
    Q_OBJECT
public:
    explicit GuhButton(QObject *parent = 0, int gpio = 4);
    bool enable();
    void disable();

private:
    GpioMonitor *m_monitor;
    int m_gpioPin;
    Gpio *m_gpio;

    bool m_buttonPressed;
    QTimer *m_longpressedTimer;

signals:
    void buttonPressed();
    void buttonReleased();
    void buttonLongPressed();

private slots:
    void gpioChanged(const int &gpioPin, const int &value);
};

#endif // GUHBUTTON_H
