#include "app.h"

#include <memory>

#include <QQmlContext>
#include <QQuickWindow>
#include <QQuickItem>
#include <QDebug>
#include <QQmlFileSelector>
#include <QQmlApplicationEngine>
#include <QApplication>

#include "manager.h"
#include "vtkitem.h"

using namespace DS;

App::App(int &argc, char **argv)
{
    QApplication::setOrganizationName("Andrei Barkhatov");
    QApplication::setApplicationName("dollstudio");
    QApplication::setApplicationDisplayName("Dolls-Studio 3D");
    QApplication::setApplicationVersion("0.1");

    _application = std::make_unique<QGuiApplication>(argc, argv);
    _qmlEng = std::make_unique<QQmlApplicationEngine>();
    _manager = std::make_unique<Manager>();
    _treemodel = std::make_unique<QStandardItemModel>();

    qmlRegisterType<Manager>(  "Dollstudio", 1, 0, "Manager");
    qmlRegisterType<VtkItem>(  "Dollstudio", 1, 0, "VtkItem");
    qmlRegisterType<QStandardItemModel>("Dollstudio", 1, 0, "TreeModel");

 #if defined(Q_OS_WIN) || defined(Q_OS_MACOS)
    QQmlFileSelector fileSelector(_qmlEng.get());
    fileSelector.setExtraSelectors(QStringList() << QLatin1String("nativemenubar"));
#endif
    _qmlEng->setInitialProperties({
        { "projectManager", QVariant::fromValue(_manager.get()) },
        { "treeModel", QVariant::fromValue(_treemodel.get()) },
    });
    _qmlEng->load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (_qmlEng->rootObjects().isEmpty()) {
        qDebug() << "... failed to load main.qml";
        exit(-1);
    }
    else {
        QObject *root = _qmlEng->rootObjects().at(0);
        VtkItem* vtk = nullptr;
        if (root) {
            vtk = reinterpret_cast<VtkItem*>(root->findChild<QObject*>("vtkItem"));
            if (vtk) {
                _manager->_vtk = vtk;
                vtk->_app = this;
                vtk->setupOpt();
            }
            else {
                qDebug() << "... failed to get vtkItem";
                exit(-1);
            }
        }
    }
}

App::~App()
{
    _qmlEng.reset();
}
