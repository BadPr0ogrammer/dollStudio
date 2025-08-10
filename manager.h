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
class Settings;
class VtkItem;
class Manager : public QObject
{
	Q_OBJECT
    QML_ELEMENT
	Q_PROPERTY(double sliderVal READ sliderVal WRITE setSliderVal NOTIFY sliderValChanged)
    Q_PROPERTY(QStandardItemModel* treeModel MEMBER _treemodel NOTIFY    treeModelChanged)
    Q_PROPERTY(QStringListModel*   listModel MEMBER _listmodel NOTIFY    listModelChanged)
	Q_PROPERTY(QString playIcon MEMBER _playIcon NOTIFY playIconChanged)
public:
	Manager(QQmlEngine* engine);
	
	QStandardItemModel*	_treemodel = nullptr;
	QStringListModel*	_listmodel = nullptr;
	VtkItem*			_vtk = nullptr;
	QQuickItem*			_slider = nullptr;
    Settings*           _settings = nullptr;

	QTimer _timer;
	bool _playing = false;
	double _curtime = -1;
	double _timerg[2] = {0,0};
	bool _completed = false;
	QString _playIcon = QString("qrc:/icons/play1.png");
	
	void resetAnim();

	double _sliderval = 0.0;
	double sliderVal() const { return _sliderval; }
	void setSliderVal(double val);

	void setConnect();
	void setTreeModel(vtkF3DAssimpImporter* importer);
	void traversTree(QStandardItem* parent, const aiNode* node);

    Q_INVOKABLE void openSource(const QUrl& url);
	Q_INVOKABLE void playToggle();
	Q_INVOKABLE void treeSelChanged(const QModelIndex& idx);
	Q_INVOKABLE void onMoved(double val);
    Q_INVOKABLE void closeSource();
    Q_INVOKABLE void cameraReset();
    Q_INVOKABLE void toggleShowAxes();
	Q_INVOKABLE void toggleShowGrid();
	Q_INVOKABLE void setDirection();
signals:	
	void sliderValChanged();
    void treeModelChanged();
    void listModelChanged();
	void playIconChanged();
public slots:
	void timerSlot();
};

}
