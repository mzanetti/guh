import QtQuick 2.3

Rectangle {
    id: root

    border.width: width / 10
    border.color: "black"
    color: "transparent"
    radius: width / 4

    property int minTemp: 0
    property int maxTemp: 50
    property int percentage: 50

    property real currentTemp: (minTemp +  1.0 * (maxTemp - minTemp) * percentage / 100)

    Rectangle {
        anchors.fill: parent
        anchors.topMargin: root.height * (100 - root.percentage) / 100
        color: "black"
        radius: parent.radius
    }
}
