import QtQuick
import QtQuick.Layouts 
import QtQuick.Controls
import QtQuick.Controls.Material

import Dollstudio 1.0
import "."

ColumnLayout {

    function applyChangesToSettings() {
        options.showAxis = showAxisBox.checked
        options.storeIt();
    }

    function revertToOldSettings() {
        showAxisBox.checked = options.showAxis
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
                checked: options.showAxis
                leftPadding: 0
            }

        }
    }
}
