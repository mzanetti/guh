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

#ifndef GUHENCODER_H
#define GUHENCODER_H

#include <QObject>
#include <QDebug>
#include <QTimer>

#include "hardware/gpiomonitor.h"

class GuhEncoder : public QObject
{
    Q_OBJECT
public:
    explicit GuhEncoder(QObject *parent = 0, int gpioButton = 2, int gpioA = 3, int gpioB = 4);

    bool enable();
    void disable();

private:
    int m_gpioPinButton;
    int m_gpioPinA;
    int m_gpioPinB;

    GpioMonitor *m_monitor;

    Gpio *m_gpioA;
    Gpio *m_gpioB;
    Gpio *m_gpioButton;

    int m_stateA;
    int m_stateB;
    bool m_buttonPressed;
    int m_encodedState;

    QTimer *m_longpressedTimer;

signals:
    void increased();
    void decreased();
    void buttonPressed();
    void buttonReleased();
    void buttonLongPressed();

private slots:
    void gpioChanged(const int &gpioPin, const int &value);
};

#endif // GUHENCODER_H
