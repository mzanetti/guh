import QtQuick 2.1

Rectangle {
    id: root
    color: "blue"
    radius: width / 2

    property int value: 0

    QtObject {
        id: priv
        // value : 100 = angle: (360-90)
        property int angle: Math.max(0, Math.min(100, root.value)) * (360-90) / 100
    }

    Repeater {
        model: 4

        Rectangle {
            color: "black"
            anchors {
                centerIn: parent
                horizontalCenterOffset: height / 2
                verticalCenterOffset: height / 2
            }

            height: parent.height
            width: height

            transform: Rotation {
                angle: {
                    var startValue = 45 + 90 * (index+1);
                    return Math.max(startValue, startValue + priv.angle - 90 * index)
                }
            }
        }
    }
}
