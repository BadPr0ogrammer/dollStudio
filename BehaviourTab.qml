import QtQml.Models 
import QtQuick 
import QtQuick.Layouts 
import QtQuick.Controls 

import Dollstudio 1.0
import "."

ColumnLayout {
    id: behaviourTab

    function applyChangesToSettings() {
        settings.loadOnStartup = loadCheckBox.checked
    }

    function revertToOldSettings() {
        loadCheckBox.checked = settings.loadOnStartup
    }

    Item {
        Layout.preferredHeight: 10
    }

    ScrollView {
        objectName: "behaviourScrollView"
        clip: true
        leftPadding: 30

        ScrollBar.horizontal.policy: ScrollBar.AsNeeded

        Layout.fillWidth: true
        Layout.fillHeight: true

        GridLayout {
            columns: 2
            columnSpacing: 12
            width: parent.width

            Label {
                text: qsTr("Load last file on startup")
            }
            CheckBox {
                id: loadCheckBox
                leftPadding: 0
                checked: settings.loadOnStartup
            }
        }
    }
}
