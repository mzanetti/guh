#include "guhtouch.h"

GuhTouch::GuhTouch(QObject *parent, int gpio) :
    QObject(parent), m_gpioPin(gpio)
{
}

bool GuhTouch::enable()
{
    qDebug() << "setup touch sensor on GPIO " << m_gpioPin;
    m_monitor = new GpioMonitor(this);

    m_gpio = new Gpio(this, m_gpioPin);

    if(!m_monitor->addGpio(m_gpio)){
        return false;
    }
    connect(m_monitor, &GpioMonitor::changed, this, &GuhTouch::gpioChanged);

    m_monitor->start();
    return true;
}

void GuhTouch::disable()
{
    m_monitor->stop();
}

void GuhTouch::gpioChanged(const int &gpioPin, const int &value)
{
    if (gpioPin == m_gpioPin){
        // check touch state
        bool touchState = !QVariant(value).toBool();
        if (m_touchState != touchState) {
            if (touchState) {
                emit handDetected();
            } else {
                emit handDisappeared();
            }
            m_touchState = touchState;
        }
    }
}
