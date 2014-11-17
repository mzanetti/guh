#ifndef GUHTOUCH_H
#define GUHTOUCH_H

#include <QObject>
#include <QDebug>

#include "hardware/gpiomonitor.h"

class GuhTouch : public QObject
{
    Q_OBJECT
public:
    explicit GuhTouch(QObject *parent = 0, int gpio = 17);
    bool enable();
    void disable();

private:
    GpioMonitor *m_monitor;
    int m_gpioPin;
    Gpio *m_gpio;

    bool m_touchState;

signals:
    void handDetected();
    void handDisappeared();

private slots:
    void gpioChanged(const int &gpioPin, const int &value);

};

#endif // GUHTOUCH_H
