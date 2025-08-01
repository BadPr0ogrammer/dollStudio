import QtQuick
import QtQuick.Controls
import QtQuick.Controls.impl
import QtQuick.Layouts
import QtQuick.Dialogs
import Qt.labs.platform as Platform
import QtQuick.Controls.Material

import Dollstudio 1.0
import  "."

ApplicationWindow {
    id: window
    width: 800
    height: 600
    visible: true
    title: qsTr("Hello World")
    Material.theme: Material.Dark
    Material.accent: Material.Teal

    required property Manager projectManager
    required property Settings options
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
                snapMode: Slider.SnapAlways
                stepSize: 0.1
                value: projectManager.sliderVal
                onMoved: projectManager.onMoved(value)             
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
                        model: projectManager.treeModel
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
                    Label { color: "#009688"; text: qsTr("Offset matrix") }
                    ListView {
                        id: propertyList
                        objectName: "propertyList"
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        model: projectManager.listModel
                        delegate: Text { 
                            text: display 
                            font.family: "monospace"
                            color: "#009688"
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

    header: ToolBar {
        RowLayout {
            anchors.fill: parent
            ToolButton {
                 icon.source: "qrc:/icons/open.png" 
                 onClicked: openProjectDialog.open()
                 Layout.preferredWidth: 30
            }
            ToolButton {
                 icon.source: "qrc:/icons/play_circle.png" 
                 onClicked: projectManager.playToggle()
                 Layout.preferredWidth: 30
            }
            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
            ToolButton {
                 icon.source: "qrc:/icons/view_all.png" 
                 onClicked: projectManager.cameraReset()
                 Layout.preferredWidth: 30
            }
            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
            ToolButton {
                 icon.source: "qrc:/icons/settings.png" 
                 onClicked: optionsDialog.open()
                 Layout.preferredWidth: 30
            }
            ToolButton {
                 icon.source: "qrc:/icons/cancel.png" 
                 onClicked: projectManager.closeSource()
                 Layout.preferredWidth: 30
            }
            ToolButton {
                 icon.source: "qrc:/icons/exit.png" 
                 onClicked: Qt.quit()
                 Layout.preferredWidth: 30
            }
        }
        background: Rectangle {
            color: "#424242"
        }
    }

    Platform.FileDialog {
        id: openProjectDialog
        objectName: "openProjectDialog"        
        nameFilters: ["FBX files (*.fbx)","All files (*)"]
        onAccepted: {
            if (!projectManager.openSource(file)) openFileErrorDlg.open 
        }
    } 

    AboutDialog {
        id: aboutDialog
        parent: Overlay.overlay
        anchors.centerIn: parent
    }
    MessageDialog { 
        id: openFileErrorDlg
        buttons: MessageDialog.Ok 
        text: "Unable to open the file!" 
    }
    OptionsDialog {
        id: optionsDialog
        parent: Overlay.overlay
        anchors.centerIn: parent
    }
}
