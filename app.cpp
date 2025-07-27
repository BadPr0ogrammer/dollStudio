#include "app.h"

#include <QQmlContext>
#include <QQuickItem>
#include <QDebug>
#include <QQmlFileSelector>
#include <QApplication>

#include "manager.h"
#include "vtkitem.h"

namespace DS
{
App::App(int& argc, char** argv)
{
	QApplication::setOrganizationName("Andrei Barkhatov");
	QApplication::setApplicationName("dollstudio");
	QApplication::setApplicationDisplayName("Dolls-Studio 3D");
	QApplication::setApplicationVersion("0.1");

	_application = new QGuiApplication(argc, argv);
	_engine = new QQmlApplicationEngine();
	_manager = new Manager(this);

	qmlRegisterType<Manager>("Dollstudio", 1, 0, "Manager");
	qmlRegisterType<VtkItem>("Dollstudio", 1, 0, "VtkItem");
	qmlRegisterType<QStandardItemModel>("Dollstudio", 1, 0, "TreeModel");
	qmlRegisterType<QStringListModel>("Dollstudio", 1, 0, "ListModel");

#if defined(Q_OS_WIN) || defined(Q_OS_MACOS)
	QQmlFileSelector fileSelector(_engine);
	fileSelector.setExtraSelectors(QStringList() << QLatin1String("nativemenubar"));
#endif
	_engine->setInitialProperties({
		{ "projectManager", QVariant::fromValue(_manager) },
		{ "treeModel", QVariant::fromValue(_manager->_treemodel) },
		{ "listModel", QVariant::fromValue(_manager->_listmodel) }
		});
	_engine->load(QUrl(QStringLiteral("qrc:/main.qml")));
	if (_engine->rootObjects().isEmpty()) {
		qDebug() << "... failed to load main.qml";
		exit(-1);
	}
	else {
		QObject* root = _engine->rootObjects().at(0);
		VtkItem* vtk = nullptr;
		if (root) {
			vtk = reinterpret_cast<VtkItem*>(root->findChild<QObject*>("vtkItem"));
			if (vtk) {
				_manager->_vtk = vtk;
				vtk->_manager = _manager;
			}
		}
		if (_manager->_vtk == nullptr) {
			qDebug() << "... failed to get qml ptrs";
			exit(-1);
		}
	}
}

void App::setup()
{
	_manager->setConnect();
}

App::~App()
{
	delete _engine;
}

}
