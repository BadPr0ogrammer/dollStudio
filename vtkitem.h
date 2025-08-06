#pragma once

#include <QQuickVTKItem.h>

#include <QString>
#include <QStandardItem>
#include <QTreeView>

#include <vtkNew.h>
#include <vtkObject.h>
#include <vtkObjectFactory.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkOpenGLRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCallbackCommand.h>
#include <vtkRenderTimerLog.h>
#include <vtkProgressBarWidget.h>
#include <vtkBoundingBox.h>
#include <vtkMatrix4x4.h>
#include <vtkActor.h>
#include <vtkOrientationMarkerWidget.h>

#include "vtkF3DMetaImporter.h"
#include "vtkF3DAssimpImporter.h"
#include "vtkF3DOpenGLGridMapper.h"

namespace fs = std::filesystem;

namespace f3d {
namespace detail {
class camera_impl;
}
}

class aiNode;
namespace DS
{
class Manager;
struct VtkItem : QQuickVTKItem
{
	Q_OBJECT
public:
	struct Data : vtkObject
	{
		static Data* New();
		vtkTypeMacro(Data, vtkObject);
		struct VtkItem* _vtkItem = nullptr;
		vtkRenderWindow* _renWin = nullptr;

		vtkSmartPointer<vtkCallbackCommand>			_timercb;
		vtkSmartPointer<vtkF3DMetaImporter>			_importer;

		std::unique_ptr<f3d::detail::camera_impl>	_camera;
		vtkSmartPointer<vtkOpenGLRenderer>			_renderer;

		vtkSmartPointer<vtkOrientationMarkerWidget> _axiswidget;
		vtkSmartPointer<vtkActor>					_gridactor;
		vtkSmartPointer<vtkF3DOpenGLGridMapper>		_gridmapper;
	};
	QString _fname;

	Manager*		_manager = nullptr;
	const aiScene*	_aiscene = nullptr;

	double _gridcolor[3] = { 0.3, 0.2, 0.3 };

	vtkUserData initializeVTK(vtkRenderWindow* renderWindow) override;
	void destroyingVTK(vtkRenderWindow* renderWindow, vtkUserData userData) override;

	void openSource();
	void close();
	void play();
	void setTreeView(Data* vtk);
	void traversTree(QStandardItem* parent, const aiNode* node);
	void timerCall();
	void sliderMove();

	void sceneAdd(VtkItem::Data* vtk, std::string fname);
	void sceneClear(VtkItem::Data* vtk);
	void sceneLoad(VtkItem::Data* vtk, const std::vector<vtkSmartPointer<vtkImporter>>& importers);

	void UpdateDynamicOptions(Data* vtk, bool force);
	void ShowAxes(Data* vtk, bool show, bool force);
	void ShowGrid(Data* vtk, bool show, bool force);
	void ConfigureGridUsingCurrentActors(Data* vtk);
	vtkBoundingBox ComputeVisiblePropOrientedBounds(Data* vtk, const vtkMatrix4x4* matrix);
};
}
