import QtQuick 1.1
import guhtune 1.0

Rectangle {
    id: root
    height: 240
    width: 320
    //anchors.fill: parent
    color: "#000000"

    Component.onCompleted: root.forceActiveFocus()

    property int currentItem: 0
    property bool buttonPressed: false
    property bool selectionMode: false
    property bool sleeping: false

    Connections {
        target: controller
        onButtonPressed: root.buttonPressed = true
        onButtonReleased: { root.buttonPressed = false; root.selectionMode = false }
        onSmallStep: {
            if (!root.selectionMode) {
                root.value = root.value + (TuneUi.RotateLeft ? -1 : 1);
            }
        }
    }

    Timer {
        running: root.buttonPressed
        interval: 300
        repeat: false
        onTriggered: {
            selectionMode = true;
        }
    }

    Timer {
        id: sleepTimer
        interval: 5000
        repeat: false
        onTriggered: {
            root.sleeping = true
        }
    }

    onButtonPressedChanged: {
        if (!root.buttonPressed) {
            controller.toggle(root.currentItem)
            selectionMode = false;
        }
    }

    states: [
        State {
            name: "splash"; when: splashTimer.running
            PropertyChanges { target: splashImage; opacity: 1 }
        },
        State {
            name: "buttons"; when: !splashTimer.running
            PropertyChanges { target: rotator; opacity: 1 }
            PropertyChanges { target: valueCircle; opacity: 1 }
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
        interval: 1500
        repeat: false
        running: true
    }

    Timer {
        id: longPressedTimer
        interval: 500
        repeat: false
        running: false
    }

    property int maxSize: Math.min(root.height, root.width)
    property int value: 50
    ValueCircle {
        id: valueCircle
        height: maxSize
        width: height
        anchors.centerIn: parent
        value: root.value
        opacity: 0
    }

    Rectangle {
        id: rotator
        height: maxSize - maxSize / 10
        width: height
        radius: height / 2
        anchors.centerIn: parent
        rotation: root.currentItem * -90
        Behavior on rotation {
            RotationAnimation {
                direction: RotationAnimation.Shortest
            }
        }
        color: "black"
        border.width: 0
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
                    radius: height / 2

                    Text {
                        anchors.centerIn: parent
                        text: index
                        scale: 1 / parent.scale
                    }
                }


                property int distanceFromCenter: height / 3.5

                states: [
                    State {
                        name: "shown"; when: index == root.currentItem && !root.selectionMode
                        PropertyChanges { target: imageItem }
                    },
                    State {
                        name: "hidden"; when: !root.selectionMode && index != root.currentItem
                        PropertyChanges { target: imageItem; opacity: 0 }
                    },
                    State {
                        name: "rotating"; when: root.selectionMode
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

    Image {
        id: splashImage
        width: root.height
        height: root.height
        anchors.centerIn: parent
        source: "qrc:///images/splash.png"
        opacity: 0
    }

    DebugUi {

    }
}
