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
        anchors.fill: parent
        orientation: Qt.Vertical
        resizing: true
        handleDelegate: Rectangle {
            height: 2
            color: "black"
        }

        SplitView {
            orientation: Qt.Horizontal
            height: topSplitMaxHeight
            Layout.maximumHeight: topSplitMaxHeight
            Layout.minimumHeight: bottomSplitMinHeight
            Layout.fillHeight: true
            resizing: true
            handleDelegate: Rectangle {
                width: 2
                color: "black"
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

        TimeLine.TimeLineView{
            width: parent.width
            Layout.minimumHeight: bottomSplitMinHeight

        }

    }

    Component.onCompleted: {

    }

}
