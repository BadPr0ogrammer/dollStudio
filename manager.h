#pragma once

#include <memory>

#include <QObject>
#include <QUrl>
#include <QQuickItem>
#include <QModelIndex>
#include <QStandardItemModel>
#include <QStringListModel>

class aiScene;
class aiNode;
class vtkF3DAssimpImporter;
namespace DS
{
class App;
class VtkItem;
class Manager : public QObject
{
	Q_OBJECT
		QML_ELEMENT
		QML_UNCREATABLE("")
		Q_MOC_INCLUDE("app.h")

public:
	Manager(App* app);
	
	App*				_app = nullptr;
	QStandardItemModel*	_treemodel = nullptr;
	QStringListModel*	_listmodel = nullptr;
	VtkItem*			_vtk = nullptr;
	
	const aiScene*		_aiscene = nullptr;

	void setConnect();
	void setTreeModel(vtkF3DAssimpImporter* importer, bool clear);
	void traversTree(QStandardItem* parent, const aiNode* node);

	Q_INVOKABLE void openSource(const QUrl& url, bool clear = true);
	Q_INVOKABLE void playFlag();
	Q_INVOKABLE void treeSelChanged(const QModelIndex& idx);
};

}
