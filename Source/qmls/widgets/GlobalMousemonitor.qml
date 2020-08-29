import QtQuick 2.0

MouseArea {
    id: mouseMonitor
    anchors.fill: parent
    property var mainWindow

    function mouseClicked(x, y)
    {
        var globalPoint = mouseMonitor.mapToGlobal(x, y)
        _global_utinity_obj.focusInTimeEdit(globalPoint.x, globalPoint.y)
    }

    onClicked: {
        mouseClicked(mouseX, mouseY)
        mouse.accepted = false
    }
}
