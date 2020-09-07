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
    property string phaseincrement: '0'
    property string zoomrate: '1'
    property double phaseincrementDefault: 0.02
    property double zoomrateDefault: 0.20

    keyframableParameters: [phaseincrement, zoomrate]
    startValues: [0.5, 0.5]
    middleValues: [phaseincrementDefault, zoomrateDefault]
    endValues: [0.5, 0.5]

    width: 350
    height: 100

    Component.onCompleted: {
        if (filter.isNew) {
            filter.set(phaseincrement, phaseincrementDefault)
            filter.set(zoomrate, zoomrateDefault)
            filter.savePreset(preset.parameters)
        }
        setControls()
    }

    function setControls() {
        var position = getPosition()
        blockUpdate = true
        phaseincrementSlider.value = filter.getDouble(phaseincrement, position) * phaseincrementSlider.maximumValue
        zoomrateSlider.value = filter.getDouble(zoomrate, position) * zoomrateSlider.maximumValue
        blockUpdate = false
        enableControls(isSimpleKeyframesActive())
    }

    function enableControls(enabled) {
        phaseincrementSlider.enabled = zoomrateSlider.enabled = enabled
    }

    function updateSimpleKeyframes() {
        updateFilter(phaseincrement, phaseincrementSlider.value / phaseincrementSlider.maximumValue, phaseKeyframesButton)
        updateFilter(zoomrate, zoomrateSlider.value / zoomrateSlider.maximumValue, zoomKeyframesButton)
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
            parameters: [phaseincrement, zoomrate]
            Layout.columnSpan: 3
            onBeforePresetLoaded: {
                filter.resetProperty(phaseincrement)
                filter.resetProperty(zoomrate)
            }
            onPresetSelected: {
                setControls()
                initializeSimpleKeyframes()
            }
        }

        Label {
            text: qsTr('Speed')
            Layout.alignment: Qt.AlignRight
        }
        SliderSpinner {
            id: phaseincrementSlider
            minimumValue: 0.0
            maximumValue: 100
            stepSize: 0.01
            decimals: 2
            suffix: ' %'
            onValueChanged: updateFilter(phaseincrement, phaseincrementSlider.value / phaseincrementSlider.maximumValue, phaseKeyframesButton, getPosition())
        }
        UndoButton {
            onClicked: phaseincrementSlider.value = phaseincrementDefault * phaseincrementSlider.maximumValue
        }
        KeyframesButton {
            id: phaseKeyframesButton
            checked: filter.animateIn <= 0 && filter.animateOut <= 0 && filter.keyframeCount(phaseincrement) > 0
            onToggled: {
                enableControls(true)
                toggleKeyframes(checked, phaseincrement, phaseincrementSlider.value / phaseincrementSlider.maximumValue)
            }
        }

        Label {
            text: qsTr('Zoom')
            Layout.alignment: Qt.AlignRight
        }
        SliderSpinner {
            id: zoomrateSlider
            minimumValue: 0.0
            maximumValue: 100
            stepSize: 0.01
            decimals: 2
            suffix: ' %'
            onValueChanged: updateFilter(zoomrate, zoomrateSlider.value / zoomrateSlider.maximumValue, zoomKeyframesButton, getPosition())
        }
        UndoButton {
            onClicked: zoomrateSlider.value = zoomrateDefault * zoomrateSlider.maximumValue
        }
        KeyframesButton {
            id: zoomKeyframesButton
            checked: filter.animateIn <= 0 && filter.animateOut <= 0 && filter.keyframeCount(zoomrate) > 0
            onToggled: {
                enableControls(true)
                toggleKeyframes(checked, zoomrate, zoomrateSlider.value / zoomrateSlider.maximumValue)
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
