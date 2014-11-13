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

GuhEncoder::GuhEncoder(QObject *parent, int gpioA, int gpioB) :
    QThread(parent), m_gpioA(gpioA), m_gpioB(gpioB)
{

}

bool GuhEncoder::enable()
{
    m_mutex.lock();
    m_enabled = true;
    m_mutex.unlock();

    start();
    return true;
}

bool GuhEncoder::disable()
{
    m_mutex.lock();
    m_enabled = false;
    m_mutex.unlock();
    return true;
}

bool GuhEncoder::isAvailable()
{
    return setUpGpio();
}

bool GuhEncoder::setUpGpio()
{
    qDebug() << "setup encoder GPIO";

//    m_gpio = new Gpio(this, m_gpioPin);

//    if (!m_gpio->exportGpio() || !m_gpio->setDirection(INPUT) || !m_gpio->setEdgeInterrupt(EDGE_BOTH)) {
//        return false;
//    }
    return true;
}

void GuhEncoder::run()
{

}
