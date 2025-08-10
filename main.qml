import QtQuick
import QtQuick.Controls
import QtQuick.Controls.impl
import QtQuick.Layouts
import QtQuick.Dialogs
import Qt.labs.platform as Platform

import Dollstudio 1.0
import  "."

ApplicationWindow {
    id: window
    width: 800
    height: 600
    visible: true
    title: qsTr("Dolls-Studio 3D")

    required property Manager projectManager
    required property Settings settings
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
            Menu {
                id: recentFilesSubMenu
                objectName: "recentFilesSubMenu"
                title: qsTr("Recent Files")
                // This can use LayoutGroup if it's ever implemented: https://bugreports.qt.io/browse/QTBUG-44078
                width: 400
                enabled: recentFilesInstantiator.count > 0

                //onClosed: canvas.forceActiveFocus()

                Instantiator {
                    id: recentFilesInstantiator
                    objectName: "recentFilesInstantiator"
                    model: settings.recentFiles
                    delegate: MenuItem {
                        // We should elide on the right when it's possible without losing the styling:
                        // https://bugreports.qt.io/browse/QTBUG-70961
                        objectName: text + "MenuItem"
                        text: settings.displayableFilePath(modelData)
                        onTriggered: projectManager.openSource(modelData)
                    }

                    onObjectAdded: (index, object) => recentFilesSubMenu.insertItem(index, object)
                    onObjectRemoved: (index, object) => recentFilesSubMenu.removeItem(object)
                }

                MenuSeparator {}

                MenuItem {
                    objectName: "clearRecentFilesMenuItem"
                    //: Empty the list of recent files in the File menu.
                    text: qsTr("Clear Recent Files")
                    onTriggered: settings.clearRecentFiles()
                }
            }

            MenuSeparator {}
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
            title: qsTr("&View")
            Action {
                text: qsTr("&Axes")
                checkable: true
                checked: settings.showAxes
                onTriggered: projectManager.toggleShowAxes()
            }
            Action {
                text: qsTr("&Grid")
                checkable: true
                checked: settings.showGrid
                onTriggered: projectManager.toggleShowGrid()
            }
        }
        Menu {
            title: qsTr("&Help")
            Action {
                text: qsTr("&About")
                onTriggered: aboutDialog.open()
            }
            MenuSeparator {}
            Action {
                text: qsTr("&Settings")
                onTriggered: optionsDialog.open()
            }
        }
    }

    header: ToolBar {
        RowLayout {
            anchors.fill: parent
            ToolButton {
                 icon.source: "qrc:/icons/open2.png"
                 icon.height: 32
                 icon.width: 32
                 onClicked: openProjectDialog.open()
            }
            ToolButton {
                 icon.source: projectManager.playIcon
                 onClicked: projectManager.playToggle()
                 icon.height: 32
                 icon.width: 32
            }
            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
            ToolButton {
                 icon.source: "qrc:/icons/view_all1.png"
                 onClicked: projectManager.cameraReset()
                 icon.height: 32
                 icon.width: 32
            }
            ToolButton {
                 icon.source: "qrc:/icons/y_up_x_right.png"
                 onClicked: {
                     settings.upDirection = 1;
                     settings.rightDirection = 0;
                     projectManager.setDirection();
                 }
                 icon.height: 32
                 icon.width: 32
            }
            ToolButton {
                 icon.source: "qrc:/icons/z_up_y_left.png"
                 onClicked: {
                     settings.upDirection = 2;
                     settings.rightDirection = 4;
                     projectManager.setDirection();
                 }
                 icon.height: 32
                 icon.width: 32
            }
            ToolButton {
                 icon.source: "qrc:/icons/z_up_x_left.png"
                 onClicked: {
                     settings.upDirection = 2;
                     settings.rightDirection = 3;
                     projectManager.setDirection();
                 }
                 icon.height: 32
                 icon.width: 32
            }
            ToolButton {
                 icon.source: "qrc:/icons/grid1.png"
                 onClicked: projectManager.toggleShowGrid()
                 icon.height: 32
                 icon.width: 32
            }
            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
            ToolButton {
                 icon.source: "qrc:/icons/settings2.png" 
                 onClicked: optionsDialog.open()
                 icon.height: 32
                 icon.width: 32
            }
            ToolButton {
                 icon.source: "qrc:/icons/cancel1.png"
                 onClicked: projectManager.closeSource()
                 icon.height: 32
                 icon.width: 32
            }
            ToolButton {
                 icon.source: "qrc:/icons/info.png"
                 onClicked: aboutDialog.open()
                 icon.height: 32
                 icon.width: 32
            }
            ToolButton {
                 icon.source: "qrc:/icons/exit1.png"
                 onClicked: Qt.quit()
                 icon.height: 32
                 icon.width: 32
            }
        }
        background: Rectangle {
            color: "#eeeaed"
            border.color: "#353637"
        }
    }

    Platform.FileDialog {
        id: openProjectDialog
        objectName: "openProjectDialog"        
        nameFilters: ["FBX files (*.fbx)","All files (*)"]
        onAccepted: {
            projectManager.openSource(file);
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
        text: "Unable to open the file." 
    }
    OptionsDialog {
        id: optionsDialog
        parent: Overlay.overlay
        anchors.centerIn: parent
    }
}
