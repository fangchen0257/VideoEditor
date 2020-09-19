import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQml 2.2
import "./"
import "../component" as LibComponent

Item {
    id: root
    property int currentDuration: 0       //调整时长
    property int totalDuration: 0 //总时长
    property alias color: bgrc.color
    property alias radius: bgrc.radius
    property string textColor: "#F0F0F0"

    property int hourRadix: (60 * 60)
    property int minuteRadix: (60)
    property int secondsRadix: 1
    property int framenumber: 0

    signal  editFocusOut()

    function formatTime(time)
    {
        return time > 9 ? '' + time : '0' + time
    }

    function setTimeEditRect()
    {
        var globalPoint = timeLayout.mapToGlobal(bgrc.x, bgrc.y)
       _global_utinity_obj.setTimeCtrlRect( globalPoint.x, globalPoint.y, bgrc.width, bgrc.height )
    }

    Rectangle {
        id: bgrc
        anchors.fill: parent

        Row {
            id: timeLayout
            spacing: 1
            padding: 4

            //hour
            PlayTimeEdit {
                id: hourDisplay
                validator: RegExpValidator{
                    regExp:/[0-9][0-9]/
                }

                onFocusChanged: {
                    if(!focus){
                        editFocusOut()
                    }
                }
            }

            DurationSplitItem {

            }

            //min
            PlayTimeEdit{
                id: minuteDisplay
                validator: RegExpValidator{
                    regExp:/[0-5][0-9]/
                }

                onFocusChanged: {
                    if(!focus){
                        editFocusOut()
                    }
                }
            }

            DurationSplitItem {

            }

            //sec
            PlayTimeEdit {
                id: secondDisplay
                validator: RegExpValidator{
                    regExp:/[0-5][0-9]/
                }
                onFocusChanged: {
                    if(!focus){
                        editFocusOut()
                    }
                }
            }

            DurationSplitItem {

            }

            PlayTimeEdit{
                id: milliSecondsDisplay
                validator: RegExpValidator{
                    regExp:/[0-2][0-9]/
                }

                onFocusChanged: {
                    if(!focus){
                        editFocusOut()
                    }
                }
            }
        }

        LibComponent.StyleImageButton {
            width: 10
            height: 5
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.topMargin: 5
            bgNormal: "qrc:/res/frame_plus_normal.png"
            bgHover: "qrc:/res/frame_plus_hover_pressed.png"
            bgPressed: "qrc:/res/frame_plus_hover_pressed.png"
            bgDisabled: "qrc:/res/frame_plus_disable.png"
            onClicked: {
                /*if(currentDuration < totalDuration){
                    ++currentDuration
                    refushPlayDuration()
                }*/
            }

            //enabled: false
        }


        LibComponent.StyleImageButton {
            width: 10
            height: 5
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            bgNormal: "qrc:/res/frame_minus_normal.png"
            bgHover: "qrc:/res/frame_minus_hover_pressed.png"
            bgPressed: "qrc:/res/frame_minus_hover_pressed.png"
            bgDisabled: "qrc:/res/frame_minus_disable.png"
            onClicked: {
                /*if(currentDuration > 0)
                {
                    --currentDuration
                    refushPlayDuration()
                }*/

            }
            //enabled: false
        }

    }

    function refushPlayDuration()
    {
        var hour = parseInt(currentDuration / hourRadix)
        var minutes = parseInt( (currentDuration % hourRadix) / minuteRadix )
        var seconds = parseInt( ((currentDuration % hourRadix) % minuteRadix) / secondsRadix )
        var milliseconds = parseInt( framenumber )



        hourDisplay.text = formatTime(hour)
        minuteDisplay.text = formatTime(minutes)
        secondDisplay.text = formatTime(seconds)
        milliSecondsDisplay.text = formatTime(milliseconds)
    }

    function setcurrentDuration(duration,framen){
        totalDuration = duration
        currentDuration = duration
        framenumber = framen;
        refushPlayDuration()
    }


    function viewDisplayDuration(){
        return Number(hourDisplay.text) * hourRadix + Number(minuteDisplay.text) * minuteRadix
                + Number(secondDisplay.text) * secondsRadix +   Number(milliSecondsDisplay.text)
    }

    onEditFocusOut: {
        //calc real duration
        currentDuration = viewDisplayDuration()
        if( currentDuration > totalDuration )
        {
            currentDuration = totalDuration
        }
        refushPlayDuration()
    }

    Component.onCompleted: {
        _global_utinity_obj.setVideoSecond.connect(setcurrentDuration)
        setTimeEditRect()
        refushPlayDuration()
    }



}
