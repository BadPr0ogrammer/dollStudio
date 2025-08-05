import QtQuick
import QtQuick.Layouts 
import QtQuick.Controls
import QtQuick.Controls.Material

import Dollstudio 1.0
import "."

ColumnLayout {

    function applyChangesToSettings() {
        settings.showAxes = showAxesBox.checked
        settings.showGrid = showGridBox.checked
        settings.storeIt();
    }

    function revertToOldSettings() {
        showAxesBox.checked = settings.showAxes
        showGridBox.checked = settings.showGrid
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
                text: qsTr("Show axes")
            }
            CheckBox {
                id: showAxesBox
                checked: settings.showAxes
                leftPadding: 0
            }
            Label {
                text: qsTr("Show grid")
            }
            CheckBox {
                id: showGridBox
                checked: settings.showGrid
                leftPadding: 0
            }
        }
    }
}
