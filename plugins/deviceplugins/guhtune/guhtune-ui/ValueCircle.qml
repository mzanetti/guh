import QtQuick 2.1

Item {
    id: root

    property int value: 0
    property alias circleOpacity: outerCircle.opacity

    Rectangle {
        id: outerCircle
        anchors.fill: parent
        color: "#9fc84a"
        radius: width / 2
    }

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
