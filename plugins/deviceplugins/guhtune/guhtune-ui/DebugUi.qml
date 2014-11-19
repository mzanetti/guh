import QtQuick 2.1

// Debug ui
Column {
    anchors { top: parent.top; left: parent.left }
    width: root.width / 10
    spacing: width / 10
    Rectangle { width: parent.width; height: width / 4; color: "white"
        Text{ anchors.centerIn: parent; text: "press" }
        MouseArea {
            anchors.fill: parent
            onClicked: root.buttonPressed = true
        }
    }
    Rectangle { width: parent.width; height: width / 4; color: "white"
        Text{ anchors.centerIn: parent; text: "release" }
        MouseArea {
            anchors.fill: parent
            onClicked: { root.buttonPressed = false; root.selectionMode = false }
        }
    }
    Rectangle { width: parent.width; height: width / 4; color: "white"
        Text{ anchors.centerIn: parent; text: "click (press & release)" }
        MouseArea {
            anchors.fill: parent
            onClicked: { root.buttonPressed = true; root.buttonPressed = false }
        }
    }
    Rectangle { width: parent.width; height: width / 4; color: "white"
        Text{ anchors.centerIn: parent; text: "rotate left" }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (root.selectionMode) {
                    root.currentItem = (root.currentItem + 1) % 4;
                } else {
                    repeater.itemAt(root.currentItem).value++;
                }
            }
        }
    }
    Rectangle { width: parent.width; height: width / 4; color: "white"
        Text{ anchors.centerIn: parent; text: "rotate right" }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (root.selectionMode) {
                    if (root.currentItem == 0) {
                        root.currentItem = 3;
                    } else {
                        root.currentItem--;
                    }
                } else {
                    repeater.itemAt(root.currentItem).value--;
                }
            }
        }
    }
    Rectangle { width: parent.width; height: width / 4; color: "white"
        Text{ anchors.centerIn: parent; text: "wakeup" }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                root.sleeping = false;
                sleepTimer.restart();
            }
        }
    }
}
