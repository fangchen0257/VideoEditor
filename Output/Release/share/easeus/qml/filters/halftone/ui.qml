/*
 * Copyright (c) 2019 Meltytech, LLC
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import Shotcut.Controls 1.0

KeyframableFilter {
    property string dotradius: '0'
    property string cyanangle: '1'
    property string magentaangle: '2'
    property string yellowangle: '3'
    
    property double dotradiusDefault: 0.40
    property double cyanangleDefault: 0.30
    property double magentaangleDefault: 0.45
    property double yellowangleDefault: 0.25
     
    keyframableParameters: [dotradius, cyanangle, magentaangle, yellowangle]
    startValues: [0.5, 0.5, 0.5, 0.5]
    middleValues: [dotradiusDefault, cyanangleDefault, magentaangleDefault, yellowangleDefault]
    endValues: [0.5, 0.5, 0.5, 0.5]

    width: 350
    height: 175

    Component.onCompleted: {
        if (filter.isNew) {
            filter.set(dotradius, dotradiusDefault)
            filter.set(cyanangle, cyanangleDefault)
            filter.set(magentaangle, magentaangleDefault)
            filter.set(yellowangle, yellowangleDefault)
            filter.savePreset(preset.parameters)
        }
        setControls()
    }

    function setControls() {
        var position = getPosition()
        blockUpdate = true
        dotradiusSlider.value = filter.getDouble(dotradius, position) * dotradiusSlider.maximumValue
        cyanangleSlider.value = filter.getDouble(cyanangle, position) * cyanangleSlider.maximumValue
        magentaangleSlider.value = filter.getDouble(magentaangle, position) * magentaangleSlider.maximumValue
        yellowangleSlider.value = filter.getDouble(yellowangle, position) * yellowangleSlider.maximumValue
        blockUpdate = false
        enableControls(isSimpleKeyframesActive())
    }

    function enableControls(enabled) {
        dotradiusSlider.enabled = cyanangleSlider.enabled = magentaangleSlider.enabled = yellowangleSlider.enabled = enabled
    }

    function updateSimpleKeyframes() {
        updateFilter(dotradius, dotradiusSlider.value / dotradiusSlider.maximumValue, dotKeyframesButton)
        updateFilter(cyanangle, cyanangleSlider.value / cyanangleSlider.maximumValue, cyanKeyframesButton)
        updateFilter(magentaangle, magentaangleSlider.value / magentaangleSlider.maximumValue, magentaKeyframesButton)
        updateFilter(yellowangle, yellowangleSlider.value / yellowangleSlider.maximumValue, yellowKeyframesButton)
    }

    GridLayout {
        anchors.fill: parent
        anchors.margins: 8
        columns: 4

        Label {
            text: qsTr('Preset')
            Layout.alignment: Qt.AlignRight
        }
        Preset {
            id: preset
            parameters: [dotradius, cyanangle, magentaangle, yellowangle]
            Layout.columnSpan: 3
            onBeforePresetLoaded: {
                filter.resetProperty(dotradius)
                filter.resetProperty(cyanangle)
                filter.resetProperty(magentaangle)
                filter.resetProperty(yellowangle)
            }
            onPresetSelected: {
                setControls()
                initializeSimpleKeyframes()
            }
        }

        Label {
            text: qsTr('Radius')
            Layout.alignment: Qt.AlignRight
        }
        SliderSpinner {
            id: dotradiusSlider
            minimumValue: 0
            maximumValue: 100.0
            stepSize: 0.1
            decimals: 1
            suffix: ' %'
            onValueChanged: updateFilter(dotradius, dotradiusSlider.value / dotradiusSlider.maximumValue, dotKeyframesButton, getPosition())
        }
        UndoButton {
            onClicked: dotradiusSlider.value = dotradiusDefault * dotradiusSlider.maximumValue
        }
        KeyframesButton {
            id: dotKeyframesButton
            checked: filter.animateIn <= 0 && filter.animateOut <= 0 && filter.keyframeCount(dotradius) > 0
            onToggled: {
                enableControls(true)
                toggleKeyframes(checked, dotradius, dotradiusSlider.value / dotradiusSlider.maximumValue)
            }
        }

        Label {
            text: qsTr('Cyan')
            Layout.alignment: Qt.AlignRight
        }
        SliderSpinner {
            id: cyanangleSlider
            minimumValue: 0
            maximumValue: 100.0
            stepSize: 0.1
            decimals: 1
            suffix: ' %'
            onValueChanged: updateFilter(cyanangle, cyanangleSlider.value / cyanangleSlider.maximumValue, cyanKeyframesButton, getPosition())
        }
        UndoButton {
            onClicked: cyanangleSlider.value = cyanangleDefault * cyanangleSlider.maximumValue
        }
        KeyframesButton {
            id: cyanKeyframesButton
            checked: filter.animateIn <= 0 && filter.animateOut <= 0 && filter.keyframeCount(cyanangle) > 0
            onToggled: {
                enableControls(true)
                toggleKeyframes(checked, cyanangle, cyanangleSlider.value / cyanangleSlider.maximumValue)
            }
        }

Label {
            text: qsTr('Magenta')
            Layout.alignment: Qt.AlignRight
        }
        SliderSpinner {
            id: magentaangleSlider
            minimumValue: 0
            maximumValue: 100.0
            stepSize: 0.1
            decimals: 1
            suffix: ' %'
            onValueChanged: updateFilter(magentaangle, magentaangleSlider.value / magentaangleSlider.maximumValue, magentaKeyframesButton, getPosition())
        }
        UndoButton {
            onClicked: magentaangleSlider.value = magentaangleDefault * magentaangleSlider.maximumValue
        }
        KeyframesButton {
            id: magentaKeyframesButton
            checked: filter.animateIn <= 0 && filter.animateOut <= 0 && filter.keyframeCount(magentaangle) > 0
            onToggled: {
                enableControls(true)
                toggleKeyframes(checked, magentaangle, magentaangleSlider.value / magentaangleSlider.maximumValue)
            }
        }

        Label {
            text: qsTr('Yellow')
            Layout.alignment: Qt.AlignRight
        }
        SliderSpinner {
            id: yellowangleSlider
            minimumValue: 0
            maximumValue: 100.0
            stepSize: 0.1
            decimals: 1
            suffix: ' %'
            onValueChanged: updateFilter(yellowangle, yellowangleSlider.value / yellowangleSlider.maximumValue, yellowKeyframesButton, getPosition())
        }
        UndoButton {
            onClicked: yellowangleSlider.value = yellowangleDefault * yellowangleSlider.maximumValue
        }
        KeyframesButton {
            id: yellowKeyframesButton
            checked: filter.animateIn <= 0 && filter.animateOut <= 0 && filter.keyframeCount(yellowangle) > 0
            onToggled: {
                enableControls(true)
                toggleKeyframes(checked, yellowangle, yellowangleSlider.value / yellowangleSlider.maximumValue)
            }
        }
        
        Item {
            Layout.fillHeight: true
        }
    }

    Connections {
        target: filter
        onInChanged: updateSimpleKeyframes()
        onOutChanged: updateSimpleKeyframes()
        onAnimateInChanged: updateSimpleKeyframes()
        onAnimateOutChanged: updateSimpleKeyframes()
    }

    Connections {
        target: producer
        onPositionChanged: setControls()
    }
}
