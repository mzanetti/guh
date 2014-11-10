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

#include "coloranimation.h"

ColorAnimation::ColorAnimation(ActionId actionId, int channel, QColor startColor, QColor endColor, int duration) :
    m_actionId(actionId), m_channel(channel), m_startColor(startColor), m_endColor(endColor), m_duration(duration)
{

    //qDebug() << "Start animation" << m_startColor.toRgb() << " --> " << m_endColor.toRgb() << "duration" << duration;

    m_animationTimer = new QTimer(this);
    m_animationTimer->setInterval(30);

    m_currentColor = m_startColor;

    connect(m_animationTimer, &QTimer::timeout, this, &ColorAnimation::animate);
}

void ColorAnimation::startAnimation()
{
    m_animationTimer->start();
}

QColor ColorAnimation::startColor()
{
    return m_startColor;
}

QColor ColorAnimation::endColor()
{
    return m_endColor;
}

int ColorAnimation::duration()
{
    return m_duration;
}

int ColorAnimation::channel()
{
    return m_channel;
}

void ColorAnimation::animate()
{
    int steps = m_duration / m_animationTimer->interval();

    qDebug() << "Steps" << steps;

    QColor newColor;
    int stepSize = (m_endColor.red() - m_startColor.red()) / steps;
    if (stepSize > 0) {
        newColor.setRed(qMin(m_currentColor.red() + stepSize, m_endColor.red()));
    } else {
        newColor.setRed(qMax(m_currentColor.red() + stepSize, m_endColor.red()));
    }
    qDebug() << "Step size R" << stepSize;


    stepSize = (m_endColor.green() - m_startColor.green()) / steps;
    if (stepSize > 0) {
        newColor.setGreen(qMin(m_currentColor.green() + stepSize, m_endColor.green()));
    } else {
        newColor.setGreen(qMax(m_currentColor.green() + stepSize, m_endColor.green()));
    }
    qDebug() << "Step size G" << stepSize;

    stepSize = (m_endColor.blue() - m_startColor.blue()) / steps;
    if (stepSize > 0) {
        newColor.setBlue(qMin(m_currentColor.blue() + stepSize, m_endColor.blue()));
    } else {
        newColor.setBlue(qMax(m_currentColor.blue() + stepSize, m_endColor.blue()));
    }
    qDebug() << "Step size B" << stepSize;


    m_currentColor = newColor;
    emit updateColor(m_channel, newColor);

    if (m_currentColor == m_endColor) {
        // animation finished
        m_animationTimer->stop();
        emit animationFinished(m_actionId);
    }
}
