#ifndef TUNEUI_H
#define TUNEUI_H

#include <QObject>

class QDeclarativeView;

class TuneUi : public QObject
{
    Q_OBJECT
    Q_ENUMS(TuneUi)

public:
    enum Rotation {
        RotationLeft,
        RotationRight
    };

    explicit TuneUi(QObject *parent = 0);
    ~TuneUi();

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
    QDeclarativeView *m_view;
};

#endif // TUNEUI_H
