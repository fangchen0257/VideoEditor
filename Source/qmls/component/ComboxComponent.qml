import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQml 2.2
ComboBox {

   id: control
   delegate: ItemDelegate {
       width: control.width
       height: 32
       contentItem: Text {
           text: modelData
           color: Qt.rgba(255, 255, 255, 0.8)
           font: control.font
           elide: Text.ElideRight
           verticalAlignment: Text.AlignVCenter
           horizontalAlignment: Text.AlignHCenter
           renderType: Text.NativeRendering
       }

       background: Rectangle {
           width: control.width
           color: control.highlightedIndex == index ? "#404040" : "#282828"
           border.color: "#282828"
           radius: 0
       }
   }

   indicator: Image {
       width: 10
       height: 5
       x: control.width - 15
       y: control.height / 2 - 5 / 2
       id: name
       source: control.down ?  "qrc:/res/frame_plus_normal.png" : "qrc:/res/frame_minus_normal.png"
   }

   contentItem: Text {
       text: control.displayText
       font: control.font
       color: "#f0f0f0"
       verticalAlignment: Text.AlignVCenter
       horizontalAlignment: Text.AlignHCenter
       elide: Text.ElideRight
       renderType: Text.NativeRendering
   }

   background: Rectangle {
       implicitWidth: 120
       implicitHeight: 40
       radius: 4
       color: "#242424"
   }


   popup: Popup {
       y: control.height
       width: control.width
       implicitHeight: contentItem.implicitHeight
       height: 104
       padding: 0

       contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            model: control.popup.visible ? control.delegateModel : null
            currentIndex: control.highlightedIndex
        }
        background: Rectangle {
            color: "#282828"
            radius: 4
        }
   }
}
