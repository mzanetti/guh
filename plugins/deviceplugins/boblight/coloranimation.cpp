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
    m_animationTimer = new QTimer(this);
    m_animationTimer->setInterval(50);

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
    QColor newColor;
    int steps = m_duration / m_animationTimer->interval();

    // RED
    int stepSizeRed = (m_endColor.red() - m_startColor.red()) / steps;
    if (stepSizeRed > 0) {
        newColor.setRed(qMin(m_currentColor.red() + stepSizeRed, m_endColor.red()));
    } else {
        newColor.setRed(qMax(m_currentColor.red() + stepSizeRed, m_endColor.red()));
    }

    // GREEN
    int stepSizeGreen = (m_endColor.green() - m_startColor.green()) / steps;
    if (stepSizeGreen > 0) {
        newColor.setGreen(qMin(m_currentColor.green() + stepSizeGreen, m_endColor.green()));
    } else {
        newColor.setGreen(qMax(m_currentColor.green() + stepSizeGreen, m_endColor.green()));
    }

    // BLUE
    int stepSizeBlue = (m_endColor.blue() - m_startColor.blue()) / steps;
    if (stepSizeBlue > 0) {
        newColor.setBlue(qMin(m_currentColor.blue() + stepSizeBlue, m_endColor.blue()));
    } else {
        newColor.setBlue(qMax(m_currentColor.blue() + stepSizeBlue, m_endColor.blue()));
    }

    // ALPHA
    int stepSizeAlpha = (m_endColor.alpha() - m_startColor.alpha()) / steps;
    if (stepSizeAlpha > 0) {
        newColor.setAlpha(qMin(m_currentColor.alpha() + stepSizeAlpha, m_endColor.alpha()));
    } else {
        newColor.setAlpha(qMax(m_currentColor.alpha() + stepSizeAlpha, m_endColor.alpha()));
    }

    qDebug() << "update color" << m_currentColor << "  ---->  " << newColor;;

    // all steps are to small (prevent endless loop)
    if(stepSizeRed == 0 && stepSizeGreen == 0 && stepSizeBlue == 0 && stepSizeAlpha == 0){
        qDebug() << "steps to small for animation --> set color directly to endcolor " << endColor();
        newColor = m_endColor;
    }

    m_currentColor = newColor;
    emit updateColor(m_channel, newColor);

    if (m_currentColor == m_endColor) {
        // animation finished
        m_animationTimer->stop();
        emit animationFinished(m_actionId);
    }
}
