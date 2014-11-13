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
#include <QThread>
#include <QDebug>

#include "hardware/gpio.h"

class GuhEncoder : public QThread
{
    Q_OBJECT
public:
    explicit GuhEncoder(QObject *parent = 0, int gpioA = 2, int gpioB = 3);
    bool enable();
    bool disable();
    bool isAvailable();

private:
    int m_gpioA;
    int m_gpioB;

    QMutex m_mutex;
    QMutex m_isPressedMutex;

    bool m_enabled;
    bool m_isPressed;

    bool setUpGpio();

protected:
    void run() override;

signals:
    void clockwiseTick();
    void counterClockwiseTick();
    void pressed();
    void released();
};

#endif // GUHENCODER_H
