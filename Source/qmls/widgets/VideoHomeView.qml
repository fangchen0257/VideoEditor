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

    focus:true
    Keys.enabled: true;
    Keys.forwardTo: [splitView]
    Keys.onPressed: {
                console.log("22");
    }
    SplitView {
        id:splitView
        orientation: Qt.Horizontal
        height: topSplitMaxHeight
        anchors.fill: parent
        resizing: true
        handleDelegate: Rectangle {
            width: 2
            color: "#151921"
        }

        focus:true
        Keys.enabled: true;
        Keys.forwardTo: [filtersView]
        Keys.onPressed: {
            console.log("333");
        }

        Filters.FiltersView {
            id:filtersView
            width: funcViewWidth
            Layout.maximumWidth: funcViewMaxWidth
            Layout.minimumWidth: funcViewMinWidth
            height: parent.height

            focus:true
            Keys.enabled: true;
            Keys.onPressed: {
                console.log("11");
            }
        }

        Play.PlayView {
            id: playLoader
            Layout.minimumWidth: funcViewMinWidth
            Layout.fillWidth: true
            height: parent.height
        }
    }
}
