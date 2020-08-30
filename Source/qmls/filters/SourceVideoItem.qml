
import QtQuick 2.0
import "../component" as QmlComponent

Item{

    property var videoPath: ""
    property bool mouseMoveOn: false
    property int videoId: 0
    property bool selected: false
    property alias videoName: nameText.text
    id: root
    width: 120
    height: 80

    MouseArea{
        id: mouseAr
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            selected = !selected
        }
        onEntered: {
            mouseMoveOn = true
            buttonRect.visible = true
        }

        onExited: {
            var mousePosX = mouseX
            var mousePosY = mouseY
            var xpos = buttonRect.x
            var ypos = buttonRect.y
            if(mouseX > xpos && mouseX < xpos + buttonRect.width && mouseY > ypos && mouseY < ypos + buttonRect.height)
            {

            }
            else{
                mouseMoveOn = false
                buttonRect.visible = false
            }
        }
    }

    Rectangle {
        id: imageRect
        color: "#000000"
        height: 68
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        border.color: {
            if(selected || mouseMoveOn){
                "#ff3c3c"
            }
            else{
                "#000000"
            }
        }

        border.width: 1

        Row{
            anchors.centerIn: parent
            spacing: 5
            height: 32
            id: buttonRect
            visible: false
            QmlComponent.StyleImageButton{
                id: videoAddBtn
                bgNormal: "qrc:/res/videoadd.png"
                bgHover: "qrc:/res/videoadd_h.png"
                bgPressed: "qrc:/res/videoadd_h.png"
                onClicked: {
                    addVideoToPlayQueue(videoPath)
                }
            }

            QmlComponent.StyleImageButton{
                id:pipAddBtn
                bgNormal: "qrc:/res/pip_icon_add.png"
                bgHover: "qrc:/res/pip_icon_add_h.png"
                bgPressed: "qrc:/res/pip_icon_add_h.png"
                onClicked: {

                }
            }

            QmlComponent.StyleImageButton{
                id: videoDelBtn
                bgNormal: "qrc:/res/videodel.png"
                bgHover: "qrc:/res/videodel_h.png"
                bgPressed: "qrc:/res/videodel_h.png"
                onClicked: imageRect.visible = false
            }
        }
    }

    Text {
        id: nameText
        anchors.top: imageRect.bottom
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        renderType: Text.NativeRendering
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 14
        color: if(selected || mouseMoveOn){
                   "#ff3c3c"
               }
               else{
                   "#ffffff7f"
               }
    }

    function addVideoToPlayQueue(fileUrl)
    {
        _global_utinity_obj.addVideoToPlayView(fileUrl);
    }

}

