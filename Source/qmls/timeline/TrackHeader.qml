import QtQuick 2.7
import QtQuick.Controls 2.2
import "../component" as QmlComponent

Item {
    property int buttonWidth: 16
    property int buttonHeight: 16

    Rectangle {
        id:timelineOper
        anchors.fill: parent
        color: "#151921"
    }

    Row {
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 20
        height: parent.height
        spacing: 16

        QmlComponent.StyleImageButton{
            id: undoBtn
            anchors.verticalCenter: parent.verticalCenter
            width: buttonWidth
            height: buttonHeight
            bgNormal: "qrc:/res/undo_normal.png"
            bgHover: "qrc:/res/undo_hover_pressed.png"
            bgPressed: "qrc:/res/undo_hover_pressed.png"
            bgDisabled: "qrc:/res/undo_disable.png"
        }

        QmlComponent.StyleImageButton{
            id: redoBtn
            anchors.verticalCenter: parent.verticalCenter
            width: buttonWidth
            height: buttonHeight
            bgNormal: "qrc:/res/redo_normal.png"
            bgHover: "qrc:/res/redo_hover_pressed.png"
            bgPressed: "qrc:/res/redo_hover_pressed.png"
            bgDisabled: "qrc:/res/redo_disable.png"
        }

        QmlComponent.StyleImageButton{
            id: cutBtn
            anchors.verticalCenter: parent.verticalCenter
            width: buttonWidth
            height: buttonHeight
            bgNormal: "qrc:/res/split_normal.png"
            bgHover: "qrc:/res/split_hover_pressed.png"
            bgPressed: "qrc:/res/split_hover_pressed.png"
            bgDisabled: "qrc:/res/split_disable.png"
        }

        QmlComponent.StyleImageButton{
            id: deleteBtn
            anchors.verticalCenter: parent.verticalCenter
            width: buttonWidth
            height: buttonHeight
            bgNormal: "qrc:/res/delete_normal.png"
            bgHover: "qrc:/res/delete_hover_pressed.png"
            bgPressed: "qrc:/res/delete_hover_pressed.png"
            bgDisabled: "qrc:/res/delete_disable.png"
        }

        QmlComponent.StyleImageButton{
            id: editBtn
            anchors.verticalCenter: parent.verticalCenter
            width: buttonWidth
            height: buttonHeight
            bgNormal: "qrc:/res/edit_normal.png"
            bgHover: "qrc:/res/edit_hover_pressed.png"
            bgPressed: "qrc:/res/edit_hover_pressed.png"
            bgDisabled: "qrc:/res/edit_disable.png"
        }

        Rectangle{
            anchors.verticalCenter: parent.verticalCenter
            width: 1
            height: buttonHeight
            color: Qt.rgba(255, 255, 255, 0.2)
        }

        QmlComponent.CommonStyleButton {
            id:btnSpeed
            anchors.verticalCenter: parent.verticalCenter
            bgNormal: "qrc:/res/speed_normal.png"
            bgHover: "qrc:/res/speed_hover_pressed.png"
            bgPressed: "qrc:/res/speed_hover_pressed.png"
            bgDisabled: "qrc:/res/speed_disable.png"
            displayText: qsTr("modifyspeed")
            clrNormal: "#0da5ef"
            clrHover: "#ef5050"
            clrPress: "#ef5050"
            clrDisable: "#134b66"
        }

        QmlComponent.CommonStyleButton {
            id:btnClip
            anchors.verticalCenter: parent.verticalCenter
            bgNormal: "qrc:/res/cut_zoom_normal.png"
            bgHover: "qrc:/res/cut_zoom_hover_pressed.png"
            bgPressed: "qrc:/res/cut_zoom_hover_pressed.png"
            bgDisabled: "qrc:/res/cut_zoom_disable.png"
            displayText: qsTr("framecut")
            clrNormal: "#0da5ef"
            clrHover: "#ef5050"
            clrPress: "#ef5050"
            clrDisable: "#134b66"
        }

        QmlComponent.CommonStyleButton {
            id:btnFreeze
            anchors.verticalCenter: parent.verticalCenter
            bgNormal: "qrc:/res/freeze_frame_normal.png"
            bgHover: "qrc:/res/freeze_frame_hover_pressed.png"
            bgPressed: "qrc:/res/freeze_frame_hover_pressed.png"
            bgDisabled: "qrc:/res/freeze_frame_disable.png"
            displayText: qsTr("framefreeze")
            clrNormal: "#0da5ef"
            clrHover: "#ef5050"
            clrPress: "#ef5050"
            clrDisable: "#134b66"
        }

        QmlComponent.CommonStyleButton {
            id:btnVoice
            anchors.verticalCenter: parent.verticalCenter
            bgNormal: "qrc:/res/voice_normal.png"
            bgHover: "qrc:/res/voice_hover_pressed.png"
            bgPressed: "qrc:/res/voice_hover_pressed.png"
            bgDisabled: "qrc:/res/voice_disable.png"
            displayText: qsTr("voice")
            clrNormal: "#0da5ef"
            clrHover: "#ef5050"
            clrPress: "#ef5050"
            clrDisable: "#134b66"
        }

        QmlComponent.CommonStyleButton {
            id:btnTex2Voice
            anchors.verticalCenter: parent.verticalCenter
            bgNormal: "qrc:/res/text_to_voice_normal.png"
            bgHover: "qrc:/res/text_to_voice_hover_pressed.png"
            bgPressed: "qrc:/res/text_to_voice_hover_pressed.png"
            bgDisabled: "qrc:/res/text_to_voice_disable.png"
            displayText: qsTr("text2voice")
            clrNormal: "#0da5ef"
            clrHover: "#ef5050"
            clrPress: "#ef5050"
            clrDisable: "#134b66"
        }

        QmlComponent.CommonStyleButton {
            id:btnGreenScreen
            anchors.verticalCenter: parent.verticalCenter
            bgNormal: "qrc:/res/green_screen_normal.png"
            bgHover: "qrc:/res/green_screen_hover_pressed.png"
            bgPressed: "qrc:/res/green_screen_hover_pressed.png"
            bgDisabled: "qrc:/res/green_screen_disable.png"
            displayText: qsTr("greenscreen")
            clrNormal: "#0da5ef"
            clrHover: "#ef5050"
            clrPress: "#ef5050"
            clrDisable: "#134b66"
        }

        Rectangle{
            anchors.verticalCenter: parent.verticalCenter
            width: 1
            height: buttonHeight
            color: Qt.rgba(255, 255, 255, 0.2)
        }

        QmlComponent.CommonStyleButton {
            id:btnGenVideo
            anchors.verticalCenter: parent.verticalCenter
            bgNormal: "qrc:/res/gen_video_normal.png"
            bgHover: "qrc:/res/gen_video_hover_pressed.png"
            bgPressed: "qrc:/res/gen_video_hover_pressed.png"
            bgDisabled: "qrc:/res/gen_video_disable.png"
            displayText: qsTr("generate")
            clrNormal: "#0da5ef"
            clrHover: "#ef5050"
            clrPress: "#ef5050"
            clrDisable: "#134b66"
        }
    }
}
