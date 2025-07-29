#pragma once

#include <memory>

#include <QObject>
#include <QUrl>
#include <QQuickItem>
#include <QModelIndex>
#include <QStandardItemModel>
#include <QStringListModel>
#include <QTimer>

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

	Q_PROPERTY(float sliderVal READ sliderVal WRITE setSliderVal NOTIFY sliderChanged)

public:
	Manager(App* app);
	
	App*				_app = nullptr;
	QStandardItemModel*	_treemodel = nullptr;
	QStringListModel*	_listmodel = nullptr;
	VtkItem*			_vtk = nullptr;
	QQuickItem*			_slider = nullptr;
	
	QTimer _timer;
	float _step = 0;

	float sliderval = 0;
	float sliderVal() { return sliderval; }
	void setSliderVal(float val);

	void setConnect();
	void setTreeModel(vtkF3DAssimpImporter* importer, bool clear);
	void traversTree(QStandardItem* parent, const aiNode* node);

	Q_INVOKABLE void openSource(const QUrl& url, bool clear = true);
	Q_INVOKABLE void playToggle();
	Q_INVOKABLE void treeSelChanged(const QModelIndex& idx);
signals:	
	void sliderChanged();
public slots:
	void timerSlot();
};

}
