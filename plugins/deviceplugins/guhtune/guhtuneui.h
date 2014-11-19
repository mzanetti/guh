#ifndef GUHTUNEUI_H
#define GUHTUNEUI_H

#include <QObject>

class QQuickView;

class GuhTuneUi : public QObject
{
    Q_OBJECT
    Q_ENUMS(GuhTuneUi)

public:
    enum Rotation {
        RotationLeft,
        RotationRight
    };

    explicit GuhTuneUi(QObject *parent = 0);
    ~GuhTuneUi();

signals:
    // Called from ui
    void toggle(int index);
    void setValue(int index, int value);

    // Called from plugin code
    void buttonPressed();
    void buttonReleased();
    void smallStep(Rotation rotation);
    void bigStep(Rotation rotation);

private:
    QQuickView *m_view;
};

#endif // GUHTUNEUI_H
