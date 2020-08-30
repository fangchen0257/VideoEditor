import QtQuick 2.0
import QtQuick.Controls 2.0
import "../component" as QmlCompoment

Item {
    id: root
    property int buttonHeight: 40
    property int buttonWidth: 50
    property var normalColor: "lightBlue"
    property var selectColor: "red"

    signal selectedVideo()
    signal selectedPhoto
    signal selectedMusic()
    signal selectedTrans()
    signal selectedText()
    signal selectedFilter()

    Rectangle {
        anchors.fill: parent
        color: "#151921"

        Row {
            id: row
            anchors.fill: parent
            anchors.leftMargin: 10
            anchors.topMargin: 10
            spacing: 25
            QmlCompoment.NavigationStyleButton {
                width: buttonWidth
                height: buttonHeight
                checked: true
                iconSource: "qrc:/res/video_normal.png"
                selectedIconSource: "qrc:/res/video_hover_pressed.png"
                textColor: checked ? selectColor : normalColor
                displayText: qsTr("Video");
                onClicked: selectedVideo()

            }

            QmlCompoment.NavigationStyleButton {
                width: buttonWidth
                height: buttonHeight
                iconSource: "qrc:/res/photo_normal.png"
                selectedIconSource: "qrc:/res/photo_hover_pressed.png"
                textColor: checked ? selectColor : normalColor
                displayText: qsTr("Photo");
                onClicked: selectedPhoto()

            }

            QmlCompoment.NavigationStyleButton {
                width: buttonWidth
                height: buttonHeight
                iconSource: "qrc:/res/music_normal.png"
                selectedIconSource: "qrc:/res/music_hover_pressed.png"
                textColor: checked ? selectColor : normalColor
                displayText: qsTr("Music");
                onClicked: selectedMusic()

            }

            QmlCompoment.NavigationStyleButton {
                width: buttonWidth
                height: buttonHeight
                iconSource: "qrc:/res/transform_normal.png"
                selectedIconSource: "qrc:/res/transform_hover_pressed.png"
                textColor: checked ? selectColor : normalColor
                displayText: qsTr("Trans");
                onClicked: selectedTrans()

            }


            QmlCompoment.NavigationStyleButton {
                width: buttonWidth
                height: buttonHeight
                iconSource: "qrc:/res/text_normal.png"
                selectedIconSource: "qrc:/res/text_hover_pressed.png"
                textColor: checked ? selectColor : normalColor
                displayText: qsTr("Text");
                onClicked: selectedText()

            }

            QmlCompoment.NavigationStyleButton {
                width: buttonWidth
                height: buttonHeight
                iconSource: "qrc:/res/filter_normal.png"
                selectedIconSource: "qrc:/res/filter_hover_pressed.png"
                textColor: checked ? selectColor : normalColor
                displayText: qsTr("Filter");
                onClicked: selectedFilter()

            }
        }

        ButtonGroup{
            id: btnGorup
            buttons: row.children
        }
    }
}
