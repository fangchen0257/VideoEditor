import QtQuick 2.0
import "./"

Item {

    Rectangle{
        anchors.fill: parent
        color: "#212735"

        TrackHeader{
            id:trackHeader
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.right: parent.right
            height: 45
        }
    }
}
