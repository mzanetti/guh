#ifndef GUHBUTTON_H
#define GUHBUTTON_H

#include <QObject>
#include <QThread>
#include <QDebug>

#include "hardware/gpio.h"

class GuhButton : public QThread
{
    Q_OBJECT
public:
    explicit GuhButton(QObject *parent = 0, int gpio = 4);

    bool enable();
    bool disable();
    bool isAvailable();
    bool isPressed();

private:
    int m_gpioPin;
    Gpio *m_gpio;

    QMutex m_mutex;
    QMutex m_isPressedMutex;
    bool m_enabled;

    bool m_isPressed;

    bool setUpGpio();

protected:
    void run() override;

signals:
    void pressed();
    void released();

};

#endif // GUHBUTTON_H
