import QtQuick
import QtQuick.Controls
import QtQuick.Controls.impl
import QtQuick.Layouts
import Qt.labs.platform as Platform

import Dollstudio 1.0
import  "./DSqml" as DSqml

ApplicationWindow
{
    width: 800
    height: 600
    visible: true
    title: "Dolls-studio"

    required property Manager projectManager
    required property TreeModel treeModel
    required property ListModel listModel
    property alias openProjectDialog: openProjectDialog

    property var aboutDialog

    SplitView {
        id: splitView1
        objectName: "SplitView1"
        anchors.fill: parent
        orientation: Qt.Horizontal
        VtkItem {
            SplitView.fillWidth: true
            objectName: "vtkItem"
        }
        SplitView {
            id: splitView2
            objectName: "SplitView2"
            orientation: Qt.Vertical
            SplitView.minimumWidth: 200
            SplitView.preferredWidth: 240
            SplitView.maximumWidth: window.width / 3
            Page {
                id: page1
                objectName: "Page1"
                padding: 16
                topPadding: 0
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
            Page {
                id: page2
                objectName: "Page2"
                padding: 16
                topPadding: 0
                SplitView.minimumHeight:  150
                SplitView.preferredHeight: 200
                SplitView.maximumHeight: window.height / 2
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
                onTriggered: projectManager.playFlag();
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
