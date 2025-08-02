import QtQml.Models
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

import Dollstudio 1.0
import "."

Dialog {
    id: dialog
    objectName: "optionsDialog"
    modal: true
    focus: true
    implicitWidth: 500
    implicitHeight: 400
    Material.theme: Material.Dark
    Material.accent: Material.Teal

    standardButtons: Dialog.Ok | Dialog.Cancel
    
    onAccepted: applyChangesToSettings()
    // We used to use onRejected here, but if the settings changes outside of our control
    // (e.g. through being reset during testing) then some controls will contain outdated
    // values since clearChanges() won't be called in that case.
    onAboutToShow: revertToOldSettings()

    function applyChangesToSettings() {
        appearanceTab.applyChangesToSettings()
        //behaviourTab.applyChangesToSettings()
    }

    function revertToOldSettings() {
        appearanceTab.revertToOldSettings()
        //behaviourTab.revertToOldSettings()
    }
    
    header: TabBar {
        id: tabBar
        // For the Universal style.
        clip: true

        TabButton {
            objectName: "appearanceTabButton"
            text: qsTr("Appearance")
        }
    }

    StackLayout {
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        AppearanceTab {
            id: appearanceTab
        }
    }
}
