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

#include "guhencoder.h"

GuhEncoder::GuhEncoder(QObject *parent, int gpioButton, int gpioA, int gpioB) :
    QObject(parent), m_gpioPinButton(gpioButton), m_gpioPinA(gpioA), m_gpioPinB(gpioB)
{
}

bool GuhEncoder::enable()
{
    qDebug() << "setup encoder A = GPIO " << m_gpioPinA << ", B = GPIO " << m_gpioPinB << ", Button = GPIO " << m_gpioPinButton;
    m_monitor = new GpioMonitor(this);

    m_gpioA = new Gpio(this, m_gpioPinA);
    m_gpioB = new Gpio(this, m_gpioPinB);
    m_gpioButton = new Gpio(this, m_gpioPinButton);

    if(!m_monitor->addGpio(m_gpioA) || !m_monitor->addGpio(m_gpioB) || !m_monitor->addGpio(m_gpioButton)){
        return false;
    }
    connect(m_monitor, &GpioMonitor::changed, this, &GuhEncoder::gpioChanged);

    m_longpressedTimer = new QTimer(this);
    m_longpressedTimer->setInterval(500);
    m_longpressedTimer->setSingleShot(true);
    connect(m_longpressedTimer, &QTimer::timeout, this, &GuhEncoder::buttonLongPressed);

    m_monitor->start();
    return true;
}

void GuhEncoder::disable()
{
    m_monitor->stop();
    m_longpressedTimer->stop();
}

void GuhEncoder::gpioChanged(const int &gpioPin, const int &value)
{
    if (gpioPin == m_gpioPinA) {
        // encode rotation
        if(value != m_stateA){
            int encoded = (value << 1) | m_stateB;
            int sum = (m_encodedState << 2) | encoded;
            if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
                emit increased();
            }
            if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
                emit decreased();
            }
            m_encodedState = encoded;
            m_stateA = value;
        }
    } else if (gpioPin == m_gpioPinB) {
        // encode rotation
        if(value != m_stateB){
            int encoded = (m_stateA << 1) | value;
            int sum = (m_encodedState << 2) | encoded;
            if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
                emit increased();
            }
            if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
                emit decreased();
            }
            m_encodedState = encoded;
            m_stateB = value;
        }
    } else if (gpioPin == m_gpioPinButton){
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

