import QtQuick
import QtQuick.Controls
import QtQuick.Controls.impl
import QtQuick.Layouts
import Qt.labs.platform as Platform

import Dollstudio 1.0
import  "./DSqml" as DSqml

ApplicationWindow {
    id: window
    width: 800
    height: 600
    visible: true
    title: qsTr("Hello World")

    required property Manager projectManager
    required property TreeModel treeModel
    required property ListModel listModel
    property alias openProjectDialog: openProjectDialog

    property var aboutDialog

    SplitView {
        id: split1
        anchors.fill: parent
        leftPadding: 4

        ColumnLayout {
            id: column1
            SplitView.fillWidth: true
            
            Frame {
                id: frame1
                Layout.fillWidth: true
                Layout.fillHeight: true
                rightPadding: 4
                leftPadding: 0
                bottomPadding: 4

                VtkItem {                
                    objectName: "vtkItem"
                    anchors.fill: parent
                }
            }
            Slider {
                id: slider1
                objectName: "slider1"
                Layout.fillWidth: true
                rightPadding: 4
                bottomPadding: 4
                height: 30
                from: 0.0
                to: 100.0
                value: projectManager.sliderVal
                onMoved: projectManager.sliderValue = value
            }
        }

        SplitView {
            id: splitView2
            orientation: Qt.Vertical
            SplitView.preferredWidth: 200
            SplitView.minimumWidth: 100

            Frame {
                id: frame2
                SplitView.fillHeight: true

                ColumnLayout {
                    anchors.fill: parent
                    Label { id: label1; color: "#606060"; text: qsTr("Skeleton"); Layout.fillHeight: false; }
                    TreeView {
                        id: treeView
                        objectName: "treeView"
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        model: treeModel
                        delegate: TreeViewDelegate { text: model.display }
                        selectionModel: ItemSelectionModel { id: selectionModel }
                        onCurrentRowChanged: { projectManager.treeSelChanged(selectionModel.currentIndex) }
                    }                    
                }
            }
            Frame {
                id: frame3
                SplitView.preferredHeight: 200
                SplitView.minimumHeight: 100

                ColumnLayout {
                    anchors.fill: parent
                    Label { color: "#606060"; text: qsTr("Offset matrix") }
                    ListView {
                        id: propertyList
                        objectName: "propertyList"
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        model: listModel 
                        delegate: Text { 
                            text: display 
                            font.family: "Courier New"
                        }
                    }
                }
            }
        }
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            Action {
                text: qsTr("&Open...")
                onTriggered: openProjectDialog.open()
            }
            MenuSeparator { }
            Action {
                text: qsTr("&Quit")
                onTriggered: Qt.quit()
            }
        }
        Menu {
            title: qsTr("&Animation")
            Action {
                text: qsTr("&Toggle")
                onTriggered: projectManager.playToggle();
            }
        }
        Menu {
            title: qsTr("&Help")
            Action {
                text: qsTr("&About")
                onTriggered: aboutDialog.open()
            }
        }
    }

    Platform.FileDialog {
        id: openProjectDialog
        objectName: "openProjectDialog"        
        nameFilters: ["FBX files (*.fbx)","All files (*)"]
        onAccepted: projectManager.openSource(file);
    } 

    DSqml.AboutDialog {
        id: aboutDialog
        parent: Overlay.overlay
        anchors.centerIn: parent
    }
}
