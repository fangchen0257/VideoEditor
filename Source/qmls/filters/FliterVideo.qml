import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQml 2.0
import Qt.labs.platform 1.0
import "./"
import "../component" as LibQmlComponent
Item {
    id: root
     property int buttonHeight: 40

    Rectangle {
        id: navigateBar
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: 100
        color: "#212735"

        Row {
            id: navigateBarLayout
            anchors.fill: parent
            Button {
                Text {
                    anchors.fill: parent
                    text: qsTr("LocalVideo")
                    color: parent.checked ? Qt.rgba(0,200,204, 1.0) : Qt.rgba(255, 255, 255, 0.9)
                    font.pixelSize: 14
                    renderType: Text.NativeRendering
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
                checked: true
                height: buttonHeight
                width: parent.width
                background: Rectangle{
                    anchors.fill: parent
                    color: "#000000000"
                }
            }

        }

    }

    Rectangle {
        id: splitVLine
        anchors.left: navigateBar.right
        anchors.top: parent.top
        width: 1
        anchors.bottom: parent.bottom
        color: "#151921"
    }

    ButtonGroup{
        buttons: navigateBarLayout.children
    }

    //right video modelView

    FileDialog {
        id: fileOpenDialog
        fileMode: FileDialog.OpenFiles
        options: FileDialog.ReadOnly
        nameFilters: "*.*"
        onAccepted: {
            createVideoItem(currentFiles)
        }
    }
    Rectangle {
        id: videoMenu
        anchors.left: splitVLine.right
        anchors.top: parent.top
        anchors.right: parent.right
        height: 40
        color: "#212735"

        LibQmlComponent.CommonStyleButton{
            height: 30
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.verticalCenter: parent.verticalCenter
            bgNormal: "qrc:/res/import_normal.png"
            bgHover: "qrc:/res/import_hover.png"
            bgPressed: "qrc:/res/import_pressed.png"
            clrNormal: "#ffffff"
            displayText: qsTr("Import")
            clrHover: Qt.rgba(0,200,204, 1.0)
            clrPress: Qt.rgba(0,200,204, 0.7)
            onClicked: {
               fileOpenDialog.open() 
           }
        }

    }   //end

    Rectangle {
        id: splitHLine
        anchors.left: splitVLine.right
        anchors.top: videoMenu.bottom
        anchors.right: parent.right
        height: 1
        color: "#151921"
    }

    // video show view
    Rectangle {
        anchors.left: splitVLine.right
        anchors.top: splitHLine.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        color: "#212735"
        Flow{
            id: videoLayout
            spacing: 10
            anchors.margins: 10
            anchors.fill: parent
        }
    }

    function createVideoItem(pathList){

        for(var index = 0; index < pathList.length; ++index)
        {
            var itemObj = Qt.createComponent("qrc:/qmls/filters/SourceVideoItem.qml")
            if(itemObj.status === Component.Ready)
            {
                var videoItem = itemObj.createObject(videoLayout)
                var fileUrl = pathList[index]
                videoItem.videoPath = fileUrl
                var fileSplit = fileUrl.split('/')
                if(fileSplit.length > 0){
                    videoItem.videoName = fileSplit[fileSplit.length - 1]
                }
                else{
                    videoItem.videoName = fileUrl
                }
            }
        }
    }

}
