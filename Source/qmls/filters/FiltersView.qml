import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
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

            onSelectedVideo: tabManager.currentIndex = 0
            onSelectedPhoto: tabManager.currentIndex = 1
            onSelectedMusic: tabManager.currentIndex = 2
            onSelectedTrans: tabManager.currentIndex = 3
            onSelectedText:  tabManager.currentIndex = 4
            /*onSelectedFilter: navigationView.push(videoFliter)*/
        }
    }

    TabView{
        id: tabManager
        anchors.left: parent.left
        anchors.top: navigation.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        style: TabViewStyle  {
                    tab: Rectangle {
                        implicitWidth: 0
                        implicitHeight: 0
                    }
                }
        Tab{
            FliterVideo{
            }
        }

        Tab{
            FilterPhoto{
            }
        }

        Tab{
            FilterMusic{}
        }

        Tab{
            FilterTransition{
            }
        }

        Tab{
            FilterText{

            }
        }

    }

    Component.onCompleted: {

    }

}
