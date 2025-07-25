import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs

import Dollstudio 1.0

Dialog {
    id: root
    objectName: "aboutDialog"
    modal: true
    dim: false
    focus: true
    standardButtons: Dialog.Ok
    width: Math.max(implicitWidth, 340)

    ColumnLayout {
        spacing: 12
        Label {
            text: qsTr("Dolls-Studio %1").arg(Qt.application.version)
            font.bold: true
            font.pixelSize: Qt.application.font.pixelSize * 1.1
            Layout.fillWidth: true
            Layout.leftMargin: 40
        }
        Label {
            text: qsTr("The app for 3D animation")
            font.pixelSize: Qt.application.font.pixelSize
            Layout.fillWidth: true
            Layout.leftMargin: 40
        }
        Label {
            text: qsTr("Copyright 2025, Andrei Barkhatov")
        }
    }
}
