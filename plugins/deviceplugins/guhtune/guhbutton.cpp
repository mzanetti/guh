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

