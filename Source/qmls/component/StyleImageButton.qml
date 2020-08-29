import QtQuick 2.0
import QtQuick.Controls 2.0

Button{

    id: root
    visible: true
    hoverEnabled: true
    property var bgNormal
    property var bgHover
    property var bgPressed
    property var bgDisabled
    onEnabledChanged: {
        if (!enabled){
            bgImage.source = bgDisabled
        }
    }

    background: Image {
        id: bgImage
        anchors.fill: parent
        source: {
            root.hovered ? (root.pressed ? bgPressed : bgHover) : bgNormal
        }
    }

}
