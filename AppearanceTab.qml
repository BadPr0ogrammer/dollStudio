import QtQuick
import QtQuick.Layouts 
import QtQuick.Controls

import Dollstudio 1.0
import "."

ColumnLayout {

    function applyChangesToSettings() {
        if (settings.showAxes != showAxesBox.checked) {
            projectManager.toggleShowAxes()
        }
        if (settings.showGrid != showGridBox.checked) {
            projectManager.toggleShowGrid()
        }
        if (settings.upDirection != upDirectionCBox.currentIndex
        ||  settings.rightDirection != rightDirectionCBox.currentIndex) {
            settings.upDirection = upDirectionCBox.currentIndex;
            settings.rightDirection = rightDirectionCBox.currentIndex;
            projectManager.setDirection()
        }
        settings.storeIt();
    }

    function revertToOldSettings() {
        showAxesBox.checked = settings.showAxes
        showGridBox.checked = settings.showGrid
        upDirectionCBox.currentIndex = settings.upDirection
        rightDirectionCBox.currentIndex = settings.rightDirection
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
            Label {
                text: qsTr("Up direction")
            }
            ComboBox {
                id: upDirectionCBox
                objectName: "upDirectionCBox"
                Component.onCompleted: currentIndex = settings.upDirection
                model: ["+X","+Y","+Z","-X","-Y","-Z"]
            }
            Label {
                text: qsTr("Right direction")
            }
            ComboBox {
                id: rightDirectionCBox
                objectName: "rightDirectionCBox"
                Component.onCompleted: currentIndex = settings.rightDirection
                model: ["+X","+Y","+Z","-X","-Y","-Z"]
            }
        }
    }
}
