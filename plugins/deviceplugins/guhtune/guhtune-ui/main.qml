import QtQuick 1.1
import guhtune 1.0

Rectangle {
    id: root
    width: 640
    height: 480
    anchors.fill: parent
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
                root.value = Math.min(100, Math.max(0, root.value + (TuneUi.RotateLeft ? -1 : 1)));
                controller.setValue(root.value)
            }
        }
        onBigStep: {
            if (root.selectionMode) {
                if (TuneUi.RotateLeft) {
                    root.currentItem = (root.currentItem + 1) % 4;
                } else {
                    if (root.currentItem == 0) {
                        root.currentItem = 3;
                    } else {
                        root.currentItem--;
                    }
                }
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
        repeat: repeat
        running: !root.selectionMode
        onTriggered: {
            root.sleeping = true
        }
    }

    onButtonPressedChanged: {
        if (!root.buttonPressed) {
            controller.toggle(root.currentItem)
            selectionMode = false;
        }
        root.sleeping = false;
        sleepTimer.restart();
    }
    onValueChanged: {
        root.sleeping = false;
        sleepTimer.restart();
    }

    states: [
        State {
            name: "splash"; when: splashTimer.running && !root.sleeping
            PropertyChanges { target: splashImage; opacity: 1 }
        },
        State {
            name: "buttons"; when: !splashTimer.running && !root.sleeping
            PropertyChanges { target: rotator; opacity: 1 }
            PropertyChanges { target: valueCircle; circleOpacity: root.selectionMode ? 0 : 1 }
        },
        State {
            name: "clock"; when: root.sleeping
            PropertyChanges { target: clock; opacity: 1 }
        }

    ]
    transitions: [
        Transition {
            from: "clock"
            to: "buttons"
            PropertyAnimation { target: splashImage; property: "opacity"; duration: 200 }
            PropertyAnimation { target: rotator; property: "opacity"; duration: 200 }
            PropertyAnimation { target: clock; property: "opacity"; duration: 200 }
            PropertyAnimation { target: valueCircle; property: "circleOpacity"; duration: 200 }
        },
        Transition {
            PropertyAnimation { target: splashImage; property: "opacity"; duration: splashTimer.interval }
            PropertyAnimation { target: rotator; property: "opacity"; duration: splashTimer.interval }
            PropertyAnimation { target: clock; property: "opacity"; duration: splashTimer.interval }
            PropertyAnimation { target: valueCircle; property: "circleOpacity"; duration: splashTimer.interval }
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
        circleOpacity: 0
        Behavior on circleOpacity {
            NumberAnimation {}
        }
    }
    Rectangle {
        anchors.centerIn: parent
        height: maxSize - maxSize / 10
        width: height
        color: "black"
        radius: height / 2
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
                Image {
                    id: image
                    anchors.centerIn: parent
                    height: parent.height
                    width: parent.width
                    source: {
                        switch(index) {
                        case 0:
                            return "qrc:///images/light.svg";
                        case 1:
                            return "qrc:///images/couch.svg";
                        case 2:
                            return "qrc:///images/work.svg";
                        case 3:
                            return "qrc:///images/template.svg";
                        }
                    }
                    rotation: index * -90 + root.currentItem * 90
                    Behavior on rotation {
                        RotationAnimation {
                            direction: RotationAnimation.Shortest
                        }
                    }
                    Behavior on scale {
                        NumberAnimation {}
                    }
                    Behavior on opacity {
                        NumberAnimation {}
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
                        PropertyChanges {
                            target: image;
                            scale: index == root.currentItem ? 0.4 : 0.3;
                            anchors.verticalCenterOffset: -distanceFromCenter
                            opacity: index == root.currentItem ? 1 : 0.8
                        }
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

    Text {
        id: clock
        Timer {
            interval: 1000
            running: true
            repeat: true
            onTriggered: {
                var now = new Date()
                clock.text = now.getHours() + ":" + now.getMinutes()
            }
        }
        anchors.centerIn: parent
        color: "white"
        opacity: 0
    }

    Image {
        id: splashImage
        width: root.height
        height: root.height
        anchors.centerIn: parent
        source: "qrc:///images/logo.svg"
        opacity: 0
    }

    DebugUi {

    }
}
