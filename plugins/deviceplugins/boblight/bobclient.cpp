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

#include "bobclient.h"

#include "libboblight/boblight.h"

#include <QDebug>

BobClient::BobClient(QObject *parent) :
    QObject(parent),
    m_connected(false),
    m_port(-1)
{

    m_lastSyncTime = QTime::currentTime();

    m_resyncTimer.setInterval(200);
    m_resyncTimer.setSingleShot(true);
    QObject::connect(&m_resyncTimer, SIGNAL(timeout()), SLOT(sync()));
}

bool BobClient::connect(const QString &hostname, int port)
{
    qDebug() << "Connecting to boblightd\n";
    m_boblight = boblight_init();

    //try to connect, if we can't then bitch to stderr and destroy boblight
    if (!boblight_connect(m_boblight, hostname.toLatin1().data(), port, 5000000) ||
            !boblight_setpriority(m_boblight, 1))
    {
        qDebug() << "Failed to connect:" << boblight_geterror(m_boblight);
        boblight_destroy(m_boblight);
        m_connected = false;
        return false;
    }
    qDebug() << "Connection to boblightd opened\n";
    m_hostname = hostname;
    m_port = port;
    m_connected = true;
    return true;
}

bool BobClient::connected() const
{
    return m_connected;
}

void BobClient::setPriority(int priority)
{
    qDebug() << "setting new priority:" << priority;
    qDebug() << "setting priority to" << priority << boblight_setpriority(m_boblight, priority);
}

void BobClient::setColor(int channel, QColor color)
{    
    if(channel == -1) {
        for(int i = 0; i < lightsCount(); ++i) {
            setColor(i, color);
        }
    } else {
        m_colors[channel] = color;
        //        qDebug() << "set channel" << channel << "to color" << color;
    }
}

void BobClient::setBrightness(int channel, int brightness)
{
    if (channel == -1) {
        for (int i = 0; i < lightsCount(); ++i) {
            QColor newColor = m_colors[i];
            newColor.setAlpha(brightness);
            setColor(i, newColor);
        }
    }else{
        QColor newColor = m_colors[channel];
        newColor.setAlpha(brightness);
        setColor(channel, newColor);
    }
}

void BobClient::sync()
{
    if(!m_connected) {
        qDebug() << "Not connected to boblight. Cannot sync";
        return;
    }
    if(m_lastSyncTime.addMSecs(50) > QTime::currentTime()) {
        if(!m_resyncTimer.isActive()) {
            m_resyncTimer.start();
        }
        return;
    }
    //qDebug() << "syncing";
    m_lastSyncTime = QTime::currentTime();

    for(int i = 0; i < lightsCount(); ++i) {
        //load the color into int array
        int rgb[3];
        rgb[0] = m_colors[i].red() * m_colors[i].alphaF();
        rgb[1] = m_colors[i].green() * m_colors[i].alphaF();
        rgb[2] = m_colors[i].blue() * m_colors[i].alphaF();
        //        qDebug() << "set color" << rgb[0] << rgb[1] << rgb[2];

        //set all lights to the color we want and send it
        boblight_addpixel(m_boblight, i, rgb);
    }

    if (!boblight_sendrgb(m_boblight, 1, NULL)) //some error happened, probably connection broken, so bitch and try again
    {
        qDebug() << "Boblight connection error!";
        qDebug() << boblight_geterror(m_boblight);
        boblight_destroy(m_boblight);
        m_connected = false;
        connect(m_hostname, m_port);
    }
}

int BobClient::lightsCount()
{
    return boblight_getnrlights(m_boblight);
}

QColor BobClient::currentColor(int channel)
{
    return m_colors[channel];
}
