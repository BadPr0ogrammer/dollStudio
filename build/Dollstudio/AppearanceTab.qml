import QtQuick
import QtQuick.Layouts 
import QtQuick.Controls
import QtQuick.Controls.Material

import Dollstudio 1.0
import "."

ColumnLayout {

    function applyChangesToSettings() {
        projectManager.showAxis = showAxisBox.checked
        projectManager.storeIt();
    }

    function revertToOldSettings() {
        showAxisBox.checked = projectManager.showAxis
    }

    Item {
        Layout.preferredHeight: 10
    }

    ScrollView {
        clip: true

        ScrollBar.horizontal.policy: ScrollBar.AsNeeded

        Layout.fillWidth: true
        Layout.fillHeight: true

        GridLayout {
            columns: 2
            columnSpacing: 12
            width: parent.width

            Label {
                text: qsTr("Show axis")
            }
            CheckBox {
                id: showAxisBox
                checked: projectManager.showAxis
                leftPadding: 0
            }

        }
    }
}
