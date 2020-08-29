import QtQuick 2.7

TextInput {
    id: edit
    color: "white"
    font.pixelSize: 16
    font.weight: Font.DemiBold
    bottomPadding: 1
    selectionColor: "#00A4FB"
    cursorVisible: false

    /*
    onFocusChanged: {
        if(!focus)
        {
            if(edit.text === "")
            {
                edit.text = "00"
            }
            else if(2 != edit.text.length)
            {
                edit.text = '0' + edit.text
            }
        }
    }*/

    MouseArea {
        id: editMouseArea
        anchors.fill: parent
        cursorShape: Qt.IBeamCursor
        onClicked: {
            edit.selectAll()
            edit.focus = true
        }
    }

    function focusOut(){
        edit.focus = false
    }

    Keys.onPressed: {
        if(event.key === Qt.Key_Return)
        {
            focusOut()
        }
    }

    Component.onCompleted: {
        _global_utinity_obj.timeEditFocusOut.connect(focusOut)
    }

}
