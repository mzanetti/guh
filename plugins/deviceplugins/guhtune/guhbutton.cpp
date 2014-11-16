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

#include "guhbutton.h"

GuhButton::GuhButton(QObject *parent, int gpio) :
    QObject(parent), m_gpioPin(gpio)
{
}

bool GuhButton::enable()
{
    qDebug() << "setup button on GPIO " << m_gpioPin;
    m_monitor = new GpioMonitor(this);

    m_gpio = new Gpio(this, m_gpioPin);

    if(!m_monitor->addGpio(m_gpio)){
        return false;
    }
    connect(m_monitor, &GpioMonitor::changed, this, &GuhButton::gpioChanged);

    m_longpressedTimer = new QTimer(this);
    m_longpressedTimer->setInterval(500);
    m_longpressedTimer->setSingleShot(true);
    connect(m_longpressedTimer, &QTimer::timeout, this, &GuhButton::buttonLongPressed);

    m_monitor->start();
    return true;
}

void GuhButton::disable()
{
    m_monitor->stop();
}

void GuhButton::gpioChanged(const int &gpioPin, const int &value)
{
    if (gpioPin == m_gpioPin){
        // check button state
        bool buttonState = !QVariant(value).toBool();
        if (m_buttonPressed != buttonState) {
            if (buttonState) {
                emit buttonPressed();
                m_longpressedTimer->start();
            } else {
                emit buttonReleased();
                m_longpressedTimer->stop();
            }
            m_buttonPressed = buttonState;
        }
    }
}


