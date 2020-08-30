import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0

import "../filters" as Filters
import "../play" as Play
import "../timeline" as TimeLine

Item {
    id: root

    property int funcViewMinWidth: 450
    property int funcViewMaxWidth: root.width - funcViewMinWidth
    property int funcViewWidth: root.width / 2
    property int bottomSplitMinHeight: 200
    property int topSplitMaxHeight: root.height - bottomSplitMinHeight

    SplitView {
        orientation: Qt.Horizontal
        height: topSplitMaxHeight
        anchors.fill: parent
        resizing: true
        handleDelegate: Rectangle {
            width: 2
            color: "#151921"
        }

        Filters.FiltersView {
            width: funcViewWidth
            Layout.maximumWidth: funcViewMaxWidth
            Layout.minimumWidth: funcViewMinWidth
            height: parent.height
        }

        Play.PlayView {
            id: playLoader
            Layout.minimumWidth: funcViewMinWidth
            Layout.fillWidth: true
            height: parent.height
        }
    }
}
