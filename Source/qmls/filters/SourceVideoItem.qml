
import QtQuick 2.0
import "../component" as QmlComponent

Item{

    property string videoPath: ""
    property bool mouseMoveOn: false
    property int videoId: 0
    property bool selected: false
    property alias videoName: nameText.text
    property alias voideCoverUrl: coverImage.source
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

        onDoubleClicked: {
            addVideoToPlayQueue(videoPath)
        }
    }

    Rectangle {
        id: imageRect
        color: "#000000"
        height: 60
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        border.width: 1
        border.color: {
            if(selected || mouseMoveOn){
                "#ff3c3c"
            }
            else{
                "#000000"
            }
        }

        Image {
            id: coverImage
            anchors.centerIn: parent
            width: 100
            height: 50
        }

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
                    addVideoToTrack(videoPath)
                }
            }

            QmlComponent.StyleImageButton{
                id:pipAddBtn
                bgNormal: "qrc:/res/pip_icon_add.png"
                bgHover: "qrc:/res/pip_icon_add_h.png"
                bgPressed: "qrc:/res/pip_icon_add_h.png"
                onClicked: {
                    addPIPToTrack(videoPath)
                }
            }

            QmlComponent.StyleImageButton{
                id: videoDelBtn
                bgNormal: "qrc:/res/videodel.png"
                bgHover: "qrc:/res/videodel_h.png"
                bgPressed: "qrc:/res/videodel_h.png"
                onClicked: root.visible = false
            }
        }
    }

    Text {
        id: nameText
        anchors.top: imageRect.bottom
        anchors.left: imageRect.left
        anchors.bottom: parent.bottom
        anchors.right: imageRect.right
        renderType: Text.NativeRendering
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 14
        elide: Text.ElideRight
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

    function addVideoToTrack(fileUrl)
    {
        _global_utinity_obj.addVideoToTrack(fileUrl);
    }

    function addPIPToTrack(fileUrl)
    {
        _global_utinity_obj.addPIPToTrack(fileUrl);
    }
}


