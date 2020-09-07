import QtQuick 2.0
import org.shotcut.qml 1.0

Metadata {
    type: Metadata.Filter
    name: qsTr("Crop: Source")
    mlt_service: "movit.crop"
    needsGPU: true
    qml: "ui.qml"
    allowMultiple: false
}
