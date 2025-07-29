#include <QStandardItemModel>
#include <QStringList>

#include "app.h"
#include "vtkitem.h"
#include "manager.h"

#include "assimp/mesh.h"

namespace DS
{
Manager::Manager(App* app) : _app(app)
{
	_treemodel = new QStandardItemModel(_app->_engine);
	_listmodel = new QStringListModel(_app->_engine);
}

void Manager::setConnect()
{
	_vtk->setupOpt();
	connect(&_timer, &QTimer::timeout, this, &Manager::timerSlot);
	_timer.start();
}

void Manager::openSource(const QUrl& url, bool clear)
{
	_vtk->_fname = url.toLocalFile();
	_vtk->openSource(clear);
}

void Manager::playToggle()
{
	_vtk->play();
	auto range = _vtk->_animanager->GetTimeRange();
	double d = range.second - range.first;
	_step = 1.0 / (30 * d);
}

void Manager::setTreeModel(vtkF3DAssimpImporter* importer, bool clear)
{
	if (clear)
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
	if (_vtk->_play) {
		_vtk->timerCall();
		setSliderVal(sliderval + _step);
	}
}

void Manager::setSliderVal(float val)
{
	if (sliderval - val < FLT_EPSILON) {
		sliderval = val;
		if (sliderval > 100)
			sliderval = 0;
		emit sliderChanged();
	}
}
}
