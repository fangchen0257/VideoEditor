import QtQuick 2.1
import QtQuick.Controls 2.1
import "../component" as QmlComponent

Item {
    id: root

    property int buttonWidth: 14
    property int buttonHeight: 14
    property alias productName: titleName.text
    property bool fullScreen: false

    signal sigPosChanged(int dx, int dy)
    signal showFullScreen(bool fullScreen)
    signal shouMinized()
    signal close()
    signal sigDoubleClicked(bool fullScreen)

    Rectangle{
        anchors.fill: parent
        color: Qt.rgba(255, 255, 255, 0.05)
    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton
        property int dx: 0
        property int dy: 0
        property int datx: 0
        property int daty: 0
        onPressed: { dx = mouseX; dy = mouseY }
        onPositionChanged: {
            if (!(datx == -1 * (mouseX - dx) && daty == -1 * (mouseY - dy)))
            {
                datx = mouseX - dx
                daty = mouseY - dy
                root.sigPosChanged(datx,  daty)
            }


        }
        onDoubleClicked: {
            root.fullScreen = !root.fullScreen
            root.updateMaxBtnImage(root.fullScreen)
            root.showFullScreen(root.fullScreen)
        }
   }

    Text {
        id: titleName
        height: parent.height
        anchors.leftMargin: 10
        anchors.left: parent.left
        anchors.top: parent.top
        text: productName
        color: Qt.rgba(255, 255, 255, 0.5)
        font.pixelSize: 20
        font.weight: Font.DemiBold
        verticalAlignment: Qt.AlignVCenter
        renderType: Text.NativeRendering
    }

    function updateMaxBtnImage(maximumed){
        if(maximumed)
        {
            maximizedBtn.bgNormal = "qrc:/res/max_normal.png"
            maximizedBtn.bgHover = "qrc:/res/max_hover.png"
            maximizedBtn.bgPressed = "qrc:/res/max_pressed.png"
        }
        else
        {
            maximizedBtn.bgNormal = "qrc:/res/normax_normal.png"
            maximizedBtn.bgHover = "qrc:/res/normax_hover.png"
            maximizedBtn.bgPressed = "qrc:/res/normax_pressed.png"
        }
    }

    Row {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 10
        height: parent.height
        spacing: 20

        QmlComponent.StyleImageButton{
            id: menuBtn
            anchors.verticalCenter: parent.verticalCenter
            width: buttonWidth
            height: buttonHeight
            bgNormal: "qrc:/res/main_menu_normal.png"
            bgHover: "qrc:/res/main_menu_hover.png"
            bgPressed: "qrc:/res/main_menu_pressed.png"
        }

        Rectangle{
            anchors.verticalCenter: parent.verticalCenter
            width: 1
            height: buttonHeight
            color: Qt.rgba(255, 255, 255, 0.2)
        }

        QmlComponent.StyleImageButton{
            id: minBtn
            anchors.verticalCenter: parent.verticalCenter
            width: buttonWidth
            height: buttonHeight
            bgNormal: "qrc:/res/min_normal.png"
            bgHover: "qrc:/res/min_hover.png"
            bgPressed: "qrc:/res/min_pressed.png"
            onClicked: root.shouMinized()
        }

        QmlComponent.StyleImageButton{
            id: maximizedBtn
            anchors.verticalCenter: parent.verticalCenter
            width: buttonWidth
            height: buttonHeight
            onClicked:{
                root.fullScreen = !root.fullScreen
                root.updateMaxBtnImage(root.fullScreen)
                showFullScreen(root.fullScreen)
            }
        }

        QmlComponent.StyleImageButton{
            anchors.verticalCenter : parent.verticalCenter
            width: buttonWidth
            height: buttonHeight
            bgNormal: "qrc:/res/close_normal.png"
            bgHover: "qrc:/res/close_hover.png"
            bgPressed: "qrc:/res/close_pressed.png"
            onClicked:
            {
                _global_utinity_obj.stopPlay();
                root.close();
            }
        }

    }

    Component.onCompleted: {
        root.updateMaxBtnImage(false)
    }

}
