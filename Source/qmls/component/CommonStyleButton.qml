import QtQuick 2.0
import QtQuick.Controls 2.0

Button {
    id: root
    property var bgNormal
    property var bgHover
    property var bgPressed
    property var bgDisabled
    property var clrNormal
    property var clrHover
    property var clrPress
    property var clrDisable
    property alias displayText: tx.text
    property int fontSize : 14

    width: {
        img.width + 5 + tx.width
    }
    height: 30

    Row{
        anchors.fill: parent
        spacing: 5
        Image {
            id: img
            anchors.verticalCenter: parent.verticalCenter
            source: {
                root.hovered ? (root.pressed ? bgPressed : bgHover) : bgNormal
            }
        }

        Text {
            id: tx
            anchors.leftMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: fontSize
            verticalAlignment: Qt.AlignVCenter
            horizontalAlignment: Qt.AlignHCenter
            renderType: Text.NativeRendering
            color: {
                root.hovered ? (root.pressed ? clrPress : clrHover) : clrNormal
            }
        }
    }

    background: Rectangle{
        color: "#00000000"
    }

}
