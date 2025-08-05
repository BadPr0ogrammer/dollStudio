#pragma once

#include "context.h"

#include <filesystem>
#include <memory>

#include <vtkSmartPointer.h>

class vtkRenderWindow;
class vtkOpenGLRenderer;
class vtkF3DMetaImporter;
class vtkRenderWindowInteractor;
class vtkOrientationMarkerWidget;
class vtkBoundingBox;
class vtkMatrix4x4;
class vtkGridAxesActor3D;

namespace DS {
class Settings;
}

namespace f3d
{
namespace detail
{
class camera_impl;

class window_impl
{
public:

	window_impl(DS::Settings* psettings, vtkRenderWindow* vtkwindow);

	~window_impl();

	void Initialize();
	void InitializeUpVector();
	void UpdateDynamicOptions(bool force);
	void ShowAxes(bool show, bool force = false);
	bool ShowGrid(bool show, bool force = false);
	vtkBoundingBox ComputeVisiblePropOrientedBounds(const vtkMatrix4x4* matrix);

	vtkRenderWindow*				RenWin = nullptr;
	DS::Settings*					settings = nullptr;

	std::unique_ptr<camera_impl>	Camera;
	vtkNew<vtkOpenGLRenderer>		Renderer;

	vtkSmartPointer<vtkOrientationMarkerWidget> AxisWidget;
	vtkNew<vtkGridAxesActor3D> GridAxesActor;
};

}
}
