import QtQuick 1.1

Rectangle {
    id: root
    width: 480   //240x320  128x160
    height: 640
    color: "#000000"

    Component.onCompleted: root.forceActiveFocus()

    property int currentItem: 0
    property bool buttonPressed: false

    Keys.onSpacePressed: {
        longPressedTimer.start()
        controller.invokeAction(root.currentItem, "pressed")
        root.buttonPressed = true;
    }
    Keys.onReleased: {
        if (event.key == Qt.Key_Space) root.buttonPressed = false;
        //controller.invokeAction(root.currentItem, "released");
    }
    Keys.onLeftPressed: {
        if (buttonPressed) {
            var newValue = root.currentItem - 1;
            if (newValue < 0) {
                newValue = 3
            }
            root.currentItem = newValue;
        } else {
            controller.invokeAction(root.currentItem, "decrease")
        }
    }
    Keys.onRightPressed: {
        if (buttonPressed) {
            root.currentItem = (root.currentItem+1) % 4
        } else {
            controller.invokeAction(root.currentItem, "increase")
        }
    }
    Keys.onTabPressed: {
        splashTimer.start()
    }

    states: [
        State {
            name: "splash"; when: splashTimer.running
            PropertyChanges { target: splashImage; opacity: 1 }
        },
        State {
            name: "buttons"; when: !splashTimer.running
            PropertyChanges { target: rotator; opacity: 1 }
        }
    ]
    transitions: [
        Transition {
            PropertyAnimation { target: splashImage; property: "opacity"; duration: splashTimer.interval }
            PropertyAnimation { target: rotator; property: "opacity"; duration: splashTimer.interval }
        }
    ]

    Timer {
        id: splashTimer
        interval: 2000
        repeat: false
        running: true
    }

    Timer {
        id: longPressedTimer
        interval: 500
        repeat: false
        running: false
    }

    Image {
        id: splashImage
        width: parent.width
        height: parent.height
        anchors.centerIn: parent
        source: "qrc:///images/logo.png"
        opacity: 0
    }

    Rectangle {
        id: rotator
        width: root.width * 0.8
        height: width
        radius: width / 2
        anchors.centerIn: parent
        rotation: root.currentItem * -90
        Behavior on rotation {
            RotationAnimation {
                direction: RotationAnimation.Shortest
            }
        }
        color: "transparent"
        border.width: 5
        border.color: "white"
        opacity: 0

        Repeater {
            id: repeater
            model: 4

            delegate: Item {
                id: imageItem
                anchors.fill: parent
                rotation: index * 90
                z: index == root.currentItem ? 1 : 0

                // This should be the image
                Rectangle {
                    id: image
                    anchors.centerIn: parent
                    height: parent.height
                    width: parent.width
                    rotation: index * -90 + root.currentItem * 90
                    Behavior on rotation {
                        RotationAnimation {
                            direction: RotationAnimation.Shortest
                        }
                    }


                    /// Test rectangle stuff
                    color: index == 0 ? "blue" : index == 1 ? "red" : index == 2 ? "green" : "khaki"
                    radius: width / 2

                    Text {
                        anchors.centerIn: parent
                        text: index
                        scale: 1 / parent.scale
                    }
                }


                property int distanceFromCenter: height / 3.5

                states: [
                    State {
                        name: "shown"; when: index == root.currentItem && !root.buttonPressed
                        PropertyChanges { target: imageItem }
                    },
                    State {
                        name: "hidden"; when: !root.buttonPressed && index != root.currentItem
                        PropertyChanges { target: imageItem; opacity: 0 }
                    },
                    State {
                        name: "rotating"; when: root.buttonPressed
                        PropertyChanges { target: image; scale: 0.4; anchors.verticalCenterOffset: -distanceFromCenter }
                    }
                ]

                transitions: [
                    Transition {
                        PropertyAnimation { target: imageItem; properties: "opacity,scale,anchors.verticalCenterOffset,anchors.horizontalCenterOffset" }
                        PropertyAnimation { target: image; properties: "opacity,scale,anchors.verticalCenterOffset,anchors.horizontalCenterOffset" }
                    }
                ]
            }
        }
    }
}
