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

import QtQuick 2.1
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import Shotcut.Controls 1.0

KeyframableFilter {
    property string amount: 'amount'
    property int amountDefault: 5

    keyframableParameters: [amount]
    startValues: [0]
    middleValues: [amountDefault]
    endValues: [0]

    width : 350
    height: 100

    Component.onCompleted: {
        if (filter.isNew) {
            filter.set('resource', filter.path + 'choppy.html')
            filter.set(amount, amountDefault)
            filter.savePreset(preset.parameters)
        }
        setControls();
    }

    function setControls() {
        blockUpdate = true
        amountSlider.value = filter.getDouble(amount, getPosition())
        blockUpdate = false
        enableControls(isSimpleKeyframesActive())
    }

    function enableControls(enabled) {
        amountSlider.enabled = enabled
    }

    function updateSimpleKeyframes() {
        updateFilter(amount, amountSlider.value, amountKeyframesButton)
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
            Layout.columnSpan: parent.columns - 1
            parameters: [amount]
            onBeforePresetLoaded: {
                filter.resetProperty(amount)
            }
            onPresetSelected: {
                setControls()
                initializeSimpleKeyframes()
            }
        }

        Label {
            text: qsTr('Repeat')
            Layout.alignment: Qt.AlignRight
        }
        SliderSpinner {
            id: amountSlider
            minimumValue: 0
            maximumValue: Math.round(profile.fps)
            stepSize: 1
            suffix: qsTr(' frames')
            spinnerWidth: 110
            onValueChanged: updateFilter(amount, amountSlider.value, amountKeyframesButton, getPosition())
        }
        UndoButton {
            onClicked: amountSlider.value = amountDefault
        }
        KeyframesButton {
            id: amountKeyframesButton
            checked: filter.animateIn <= 0 && filter.animateOut <= 0 && filter.keyframeCount(amount) > 0
            onToggled: {
                enableControls(true)
                toggleKeyframes(checked, amount, amountSlider.value)
            }
        }

        Item { Layout.fillHeight: true }
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
