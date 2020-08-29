import QtQuick 2.0
import "./"

Item {
    id: root

    Rectangle {
        id: navigation
        color: "#202020"
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right
        height: 60
        FilterNavigationBar{
            anchors.fill: parent

            onSelectedVideo: filterLoader.source = "qrc:/qmls/filters/FliterVideo.qml"
            onSelectedPhoto: filterLoader.source = "qrc:/qmls/filters/FilterPhoto.qml"
            onSelectedMusic: filterLoader.source ="qrc:/qmls/filters/FilterMusic.qml"
            onSelectedTrans: filterLoader.source = "qrc:/qmls/filters/FilterTransition.qml"
            onSelectedText:  filterLoader.source = "qrc:/qmls/filters/FilterText.qml"
            onSelectedFilter: filterLoader.source = "qrc:/qmls/filters/FilterFilter.qml"
        }
    }

    Loader{
        id: filterLoader
        anchors.left: parent.left
        anchors.top: navigation.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }

    Component.onCompleted: {

        filterLoader.source = "qrc:/qmls/filters/FliterVideo.qml"
    }

}
