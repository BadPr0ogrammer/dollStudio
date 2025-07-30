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
class VtkItem;
class Manager : public QObject
{
	Q_OBJECT
	Q_PROPERTY(double sliderVal READ sliderVal WRITE setSliderVal NOTIFY sliderChanged)

public:
	Manager(QQmlEngine* engine);
	
	QStandardItemModel*	_treemodel = nullptr;
	QStringListModel*	_listmodel = nullptr;
	VtkItem*			_vtk = nullptr;
	QQuickItem*			_slider = nullptr;
	
	QTimer _timer;
	double _step = 0;

	double _sliderval = 0.0;
	double sliderVal() const { return _sliderval; }
	void setSliderVal(double val);

	void setConnect();
	void setTreeModel(vtkF3DAssimpImporter* importer, bool clear);
	void traversTree(QStandardItem* parent, const aiNode* node);

	Q_INVOKABLE bool openSource(const QUrl& url, bool clear = true);
	Q_INVOKABLE void playToggle();
	Q_INVOKABLE void treeSelChanged(const QModelIndex& idx);
	Q_INVOKABLE void onMoved(double val);
signals:	
	void sliderChanged();
public slots:
	void timerSlot();
};

}
