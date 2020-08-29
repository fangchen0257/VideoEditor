import QtQuick 2.0
import QtQuick.Controls 2.0

Button {

    property var iconSource
    property var selectedIconSource
    property alias textColor: tx.color
    property var selectedTextColor
    property alias displayText: tx.text
    property int fontSize : 14
    checkable: true
    Column{
        anchors.fill: parent
        spacing: 8
        Image {
            id: img
            source: checked ? selectedIconSource : iconSource
            anchors.horizontalCenter: tx.horizontalCenter
        }

        Text {
            id: tx
            font.pixelSize: fontSize
            wrapMode: Text.WrapAnywhere
            verticalAlignment: Qt.AlignVCenter
            horizontalAlignment: Qt.AlignHCenter
            renderType: Text.NativeRendering
        }
    }

    background: Rectangle{
        color: "#00000000"
    }


}
