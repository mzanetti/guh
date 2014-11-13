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
    QThread(parent), m_gpioPin(gpio)
{
}

bool GuhButton::enable()
{
    m_mutex.lock();
    m_enabled = true;
    m_mutex.unlock();

    start();
    return true;
}

bool GuhButton::disable()
{
    m_mutex.lock();
    m_enabled = false;
    m_mutex.unlock();
    return true;
}

bool GuhButton::isAvailable()
{
    return setUpGpio();
}

bool GuhButton::isPressed()
{
    m_isPressedMutex.lock();
    return m_isPressed;
    m_isPressedMutex.unlock();
}

bool GuhButton::setUpGpio()
{
    qDebug() << "setup button GPIO";

    m_gpio = new Gpio(this, m_gpioPin);

    if (!m_gpio->exportGpio() || !m_gpio->setDirection(INPUT) || !m_gpio->setEdgeInterrupt(EDGE_BOTH)) {
        return false;
    }
    return true;
}

void GuhButton::run()
{
    bool enabled = true;
    m_mutex.lock();
    m_enabled = true;
    m_mutex.unlock();

    // poll the gpio file, if something changes...emit the signal with the current pulse length
    while (enabled) {
        m_isPressedMutex.lock();
        bool currentValue = QVariant(m_gpio->getValue()).toBool();
        if (m_isPressed != currentValue) {
            m_isPressed = currentValue;
            if (m_isPressed) {
                emit pressed();
            } else{
                emit released();
            }
        }
        m_isPressedMutex.unlock();
        usleep(100000);
    }

    m_mutex.lock();
    enabled = m_enabled;
    m_mutex.unlock();
}

