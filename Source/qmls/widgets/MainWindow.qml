import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import Play.FramePlayView 1.0
import "./"

ApplicationWindow {
    id: root
    visible: true
    width: 1280
    height: 768
    color: "#00000000"
    flags: Qt.FramelessWindowHint | Qt.Window

    property int borderRadius: 0
    property int shadowBlur: 0

    GlobalMousemonitor {
        anchors.fill: parent
        mainWindow : root
    }

/*
    DropShadow {
        id: shadow
        anchors.fill: bgrc
        color: "#444848"
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        source: bgrc
        samples: shadowBlur
        horizontalOffset: 0
        verticalOffset: 0
    }
*/
    Rectangle {
        id: bgrc
        anchors.fill: parent
        color: "#2E2F30"
        border.width: 1
        border.color: "#1884D7"
        radius: borderRadius
        anchors.margins: shadowBlur

        WindowTitle {
            id: title
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: 40
            productName: qsTr("QtX Video Edit")
            onSigPosChanged: {
                root.x += dx
                root.y += dy
            }

            onShouMinized: root.showMinimized()
            function showFullScreenFunc(full, parent)
            {
                if(full)
                {
                    root.showFullScreen()
                    parent.anchors.margins = 0
                }
                else
                {
                    root.showNormal()
                    parent.anchors.margins = shadowBlur
                }
            }

            onShowFullScreen: {
                showFullScreenFunc(fullScreen, parent)

            }

            onClose: root.close()
        }


        VideoHomeView {
            id: home
            anchors.top: title.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: 1
        }


    }

}
