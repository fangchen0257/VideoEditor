import QtQuick 2.0
import Play.FramePlayView 1.0
import QtQuick.Window 2.0
import QtQuick.Controls 2.0
import QtQuick.Dialogs 1.2

import "../component" as LibComponent
import "./play.js" as PlayUtility
Item {

    //16 : 9
    id: playItem
    property int contrlHeight: 60
    property int timeContrlWidth: 160
    property int buttonWidth: 14
    property int buttonHeight: 14
    property int curDuration: 0
    property int totalDuration: 0

    function calcRenderWidth( width, height ){
        var widthHeightPro = PlayUtility.parseWidthHeightProportion(comboxProportion.currentText)
        var total = widthHeightPro[0] + widthHeightPro[1]
        var tmpHeight = widthHeightPro[1] / widthHeightPro[0] * width
        if(tmpHeight > height)
        {
            //高越界
            renderView.height = tmpHeight
            renderView.width = height * widthHeightPro[0] / widthHeightPro[1]
        }
        else
        {
            //高未越界
            renderView.width = width
            renderView.height = tmpHeight
        }
    }

    function calcRenderHeight(width, height){
        var widthHeightPro = PlayUtility.parseWidthHeightProportion(comboxProportion.currentText)
        var total = widthHeightPro[0] + widthHeightPro[1]
        var tmpWidth = widthHeightPro[0] / widthHeightPro[1] * height
        if(tmpWidth < width)
        {
            //宽未越界
            renderView.height = height
            renderView.width = tmpWidth
        }
        else
        {
            //宽越界
            renderView.width = width
            renderView.height = width * widthHeightPro[1] / widthHeightPro[0]
        }
    }

    function calRenderViewSize(w,h)
    {
        if (w === 0 || h === 0)
            return;

        renderView.m_width =w;
        renderView.m_height = h;

        var mw = displayArea.width;
        var mh = displayArea.height;

        var ratedis = mw/mh;
        var rateimg = w/h;

        if (rateimg > ratedis)
        {
            renderView.width = mw;
            renderView.height = renderView.width/rateimg
        }
        else
        {
            renderView.height = mh;
            renderView.width = renderView.height * rateimg;
        }
    }

    Rectangle {
        id: displayArea
        anchors.top: parent.top
        anchors.left: parent.left
        height: parent.height  - contrlHeight
        width: parent.width
        color: "#404040"
        onWidthChanged: {
            //calcRenderWidth(width, height)
            //renderView.calcRenderSize(renderView.m_width,renderView.m_height)
            calRenderViewSize( renderView.m_width,  renderView.m_height )
        }

        onHeightChanged: {
            //calcRenderHeight(width, height)
            //renderView.calcRenderSize(renderView.m_width,renderView.m_height)
            calRenderViewSize( renderView.m_width,  renderView.m_height )
        }

        FramePlayView {
            id: renderView
            anchors.centerIn: parent
            property int m_width:displayArea.width
            property int m_height:displayArea.height

            function calcRenderSize( w, h )
            {
                calRenderViewSize(w,h);
            }
        }

    }

    Rectangle {
        id: controlView
        anchors.top: displayArea.bottom
        anchors.left: parent.left
        height: contrlHeight
        width: parent.width
        color: "#484848"

        Row {
            id: rowLayout
            anchors.left: parent.left
            anchors.top: parent.top
            height: 24
            spacing: 15
            width: parent.width  - timeContrlWidth
            property int margins: 10
            anchors.leftMargin: margins
            anchors.rightMargin: 0
            anchors.topMargin:  margins

            LibComponent.StyleImageButton {
                id:playBt
                width: buttonWidth
                height: buttonHeight
                bgNormal: "qrc:/res/play_normal.png"
                bgHover: "qrc:/res/play_hover_pressed.png"
                bgPressed: "qrc:/res/play_hover_pressed.png"
                onClicked:
                {
                    _global_utinity_obj.setPlay(true);
                    visible = false;
                    pauseBt.visible = true;
                }

            }
            LibComponent.StyleImageButton {
                id:pauseBt
                width: buttonWidth
                height: buttonHeight
                bgNormal: "qrc:/res/pause_normal.png"
                bgHover: "qrc:/res/pause_hover_pressed.png"
                bgPressed: "qrc:/res/pause_hover_pressed.png"
                visible: false
                onClicked:
                {
                    _global_utinity_obj.setPlay(false);
                    visible = false;
                    playBt.visible = true;
                }
            }

            LibComponent.StyleImageButton{
                width: buttonWidth
                height: buttonHeight
                bgNormal: "qrc:/res/backward_normal.png"
                bgHover: "qrc:/res/backward_hover_pressed.png"
                bgPressed: "qrc:/res/backward_hover_pressed.png"
                onClicked:
                {
                    if (playItem.curDuration > 5)
                    {
                        var v = playItem.curDuration - 5;
                        _global_utinity_obj.seekToPos(v);
                        playItem.curDuration = v;
                        progress.value = v;
                        durationEdit.setcurrentDuration(v);
                    }
                }
            }

            LibComponent.StyleImageButton{
                id: nextFrame
                width: buttonWidth
                height: buttonHeight
                bgNormal: "qrc:/res/forward_normal.png"
                bgHover: "qrc:/res/forward_hover_pressed.png"
                bgPressed: "qrc:/res/forward_hover_pressed.png"
                onClicked:
                {
                    if (playItem.curDuration + 5 < playItem.totalDuration)
                    {
                        var v = playItem.curDuration + 5;
                        _global_utinity_obj.seekToPos(v);
                        playItem.curDuration = v;
                        progress.value = v;
                        durationEdit.setcurrentDuration(v);
                    }
                }
            }

           Slider {
               id: progress
               value: 0.0
               from: 0
               width: parent.width - 120
               height: buttonHeight
               padding: 0
               stepSize: 1
               onMoved:
               {
                    playItem.curDuration = value;
                    _global_utinity_obj.seekToPos(value);
               }

               background: Rectangle {
                        x: progress.leftPadding
                        y: progress.topPadding + progress.availableHeight / 2 - height / 2
                        width: progress.availableWidth
                        height: 2
                        radius: 1
                        color: "#00A4FB"

                        Rectangle {
                            width: progress.visualPosition * parent.width
                            height: 2
                            color: "#0082A2"
                            radius: 1
                        }
                    }

                    handle: Rectangle {
                        x: progress.leftPadding + progress.visualPosition * (progress.availableWidth - width)
                        y: progress.topPadding + progress.availableHeight / 2 - height / 2
                        implicitWidth: 14
                        implicitHeight: 14
                        radius: 7
                        color: "#323232"
                        border.color: "#00A4FB"
                        border.width: 1
                    }
             }

        }   //end column

        LibComponent.DurationItem {
                id: durationEdit
                anchors.right: parent.right
                anchors.top: parent.top
                //anchors.left: rowLayout.right
                anchors.topMargin: 5
                anchors.rightMargin: 10
                width: timeContrlWidth - 30
                height: 24
                color: "#000000"
                radius: 4

            }

        /*
        LibComponent.ComboxComponent {
            id: comboxProportion
            model: [ qsTr("16:9"), qsTr("4:3"), qsTr("9:16") ]
            anchors.left: rowLayout.left
            anchors.top: rowLayout.bottom
            anchors.margins: 0
            width: 80
            height: 20
            onActivated: {
                calcRenderWidth(displayArea.width, displayArea.height)
                calcRenderHeight(displayArea.width, displayArea.height)
            }
        }*/

       /* LibComponent.StyleImageButton{
            anchors.left: rowLayout.left
            anchors.top: rowLayout.bottom
            anchors.margins: 0
            width: 20
            height: 21
            bgNormal: "qrc:/res/import_normal.png"
            bgHover: "qrc:/res/import_hover.png"
            bgPressed: "qrc:/res/import_pressed.png"
            onClicked:
            {
                _global_utinity_obj.openFile();
            }
        }*/

        onWidthChanged: {
            durationEdit.setTimeEditRect()

        }
    }

    function initVideoDuration(duration){
        //durationEdit.setcurrentDuration(duration)
        progress.to = duration;
        playItem.totalDuration = duration;
        playBt.visible = false;
        pauseBt.visible = true;
    }

    function setVideoDuration(duration)
    {
        playItem.curDuration = duration;
        progress.value = duration;
    }

    Component.onCompleted: {
        _global_utinity_obj.setPlayObject(renderView);
        _global_utinity_obj.initVideoDuration.connect(initVideoDuration);
        _global_utinity_obj.setVideoDuration.connect(setVideoDuration);
    }
}
