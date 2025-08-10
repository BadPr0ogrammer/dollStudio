#include <QStandardItemModel>
#include <QStringList>
#include <QThread>

#include "app.h"
#include "vtkitem.h"
#include "manager.h"
#include "settings.h"

#include "camera_impl.h"

#include "assimp/mesh.h"

namespace DS
{
Manager::Manager(QQmlEngine* engine) 
{
	_treemodel = new QStandardItemModel(engine);
	_listmodel = new QStringListModel(engine);
}

void Manager::setConnect()
{
	connect(&_timer, &QTimer::timeout, this, &Manager::timerSlot);
	_timer.start();
}

void Manager::openSource(const QString& fname)
{
	if (_vtk->openSource(fname) != 0) {
		qDebug() << "adding" << fname << "to recent files list";
		_settings->addRecentFile(fname);
	}
}

void Manager::openSourceUrl(const QUrl& url)
{
	QString fname = url.toLocalFile();
	openSource(fname);
}

void Manager::playToggle()
{
	_vtk->play();
	_playIcon = _playing ? QString("qrc:/icons/play1.png") : QString("qrc:/icons/stop.png");
	emit playIconChanged();
}

void Manager::setTreeModel(vtkF3DAssimpImporter* importer)
{
	_treemodel->removeRows(0, _treemodel->rowCount());

	const aiNode* root = _vtk->_aiscene->mRootNode;
	QStandardItem* item = _treemodel->invisibleRootItem();
	item->setData(QString(root->mName.C_Str()), Qt::DisplayRole);
	item->setData(QVariant::fromValue<const aiNode*>(root));
	for (int i = 0; i < root->mNumChildren; i++)
		traversTree(item, root->mChildren[i]);
}

void Manager::traversTree(QStandardItem* parent, const aiNode* node)
{
	if (node != nullptr && !node->mNumMeshes) {		
		QStandardItem* item = new QStandardItem();
		item->setData(QString(node->mName.C_Str()), Qt::DisplayRole);
		item->setData(QVariant::fromValue<const aiNode*>(node));
		parent->appendRow(item);
		for (int i = 0; i < node->mNumChildren; i++)
			traversTree(item, node->mChildren[i]);
	}
}

void Manager::treeSelChanged(const QModelIndex& idx)
{
	if (!idx.isValid() || !idx.data(Qt::UserRole + 1).isValid()) {
		qDebug() << "Tree model index is invalid!";
		return;
	}
	const aiNode* node = idx.data(Qt::UserRole + 1).value<const aiNode*>();
	if (!node) {
		qDebug() << "Tree model data is invalid!";
		return;
	}
	QStringList list;
	list << "Node name: " + QString(node->mName.C_Str());

	std::vector<aiBone*> vbone;
	std::for_each(&_vtk->_aiscene->mMeshes[0], &_vtk->_aiscene->mMeshes[_vtk->_aiscene->mNumMeshes], [&](aiMesh* mesh) {
		std::for_each(&mesh->mBones[0], &mesh->mBones[mesh->mNumBones], [&](aiBone* bone) {
			if (bone->mNode == node)
				vbone.emplace_back(bone);
			});
		});
	std::sort(vbone.begin(), vbone.end(), [](const aiBone* a, const aiBone* b) {
		return a->mNumWeights > b->mNumWeights;
		});
	if (vbone.size()) {
		aiBone* b = vbone[0];
		auto m = b->mOffsetMatrix;
		list << QString::asprintf("%+.3f", m.a1) + ";" + QString::asprintf("%+.3f", m.a2) + ";" + QString::asprintf("%+.3f", m.a3) + ";" + QString::asprintf("%+.3f", m.a4);
		list << QString::asprintf("%+.3f", m.b1) + ";" + QString::asprintf("%+.3f", m.b2) + ";" + QString::asprintf("%+.3f", m.b3) + ";" + QString::asprintf("%+.3f", m.b4);
		list << QString::asprintf("%+.3f", m.c1) + ";" + QString::asprintf("%+.3f", m.c2) + ";" + QString::asprintf("%+.3f", m.c3) + ";" + QString::asprintf("%+.3f", m.c4);
		list << QString::asprintf("%+.3f", m.d1) + ";" + QString::asprintf("%+.3f", m.d2) + ";" + QString::asprintf("%+.3f", m.d3) + ";" + QString::asprintf("%+.3f", m.d4);
	}		
	_listmodel->setStringList(list);
}

void Manager::timerSlot()
{
	if (_playing) {
		_vtk->timerCall();
		setSliderVal(_sliderval);
	}
}

void Manager::setSliderVal(double val)
{
	if (_sliderval - val > 2*DBL_EPSILON) {
		_sliderval = val;
		if (_sliderval > 1 || _sliderval < 0)
			_sliderval = 0;
		emit sliderValChanged();
	}
}

void Manager::onMoved(double val)
{
	_sliderval = val;
	_vtk->sliderMove();
}

void Manager::closeSource()
{
    _vtk->close();

    delete _treemodel;
    _treemodel = nullptr;
    delete _listmodel;
    _listmodel = nullptr;
}

void Manager::cameraReset()
{
    _vtk->dispatch_async([&](vtkRenderWindow* renderWindow, VtkItem::vtkUserData userData) {
        VtkItem::Data* vtk = (VtkItem::Data*)userData.GetPointer();
        vtk->_camera->resetToBounds();
    });
    QThread::msleep(10);
}

void Manager::toggleShowAxes()
{
    _vtk->dispatch_async([&](vtkRenderWindow* renderWindow, VtkItem::vtkUserData userData) {
        VtkItem::Data* vtk = (VtkItem::Data*)userData.GetPointer();
		_settings->setShowAxes(!_settings->showAxes());
		vtk->_vtkItem->ShowAxes(vtk, _settings->showAxes(), true);
    });
    QThread::msleep(10);
}

void Manager::toggleShowGrid()
{
	_vtk->dispatch_async([&](vtkRenderWindow* renderWindow, VtkItem::vtkUserData userData) {
		VtkItem::Data* vtk = (VtkItem::Data*)userData.GetPointer();
		_settings->setShowGrid(!_settings->showGrid());
		vtk->_vtkItem->ShowGrid(vtk, _settings->showGrid(), true);
		});
	QThread::msleep(10);
}

void Manager::setDirection()
{
	_vtk->dispatch_async([&](vtkRenderWindow* renderWindow, VtkItem::vtkUserData userData) {
		VtkItem::Data* vtk = (VtkItem::Data*)userData.GetPointer();
		int up = _settings->upDirection();
		int right = _settings->rightDirection();
		f3d::direction_t up_direction = _settings->getDirectionFromIdx(up);
        f3d::direction_t right_direction = _settings->getDirectionFromIdx(right);

        vtk->_vtkItem->InitializeUpVector(vtk, up_direction, right_direction);
		vtk->_camera->resetToBounds();
		if (_settings->showGrid())
			vtk->_vtkItem->ShowGrid(vtk, true, true);
		});
	QThread::msleep(10);
}

void Manager::resetAnim()
{
	_playing = false;
	_curtime = 0;
}

}
