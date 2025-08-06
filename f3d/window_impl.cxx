#include "window_impl.h"

#include "camera_impl.h"

#include <vtkCamera.h>
#include <vtkImageData.h>
#include <vtkImageExport.h>
#include <vtkInformation.h>
#include <vtkPNGReader.h>
#include <vtkPointGaussianMapper.h>
#include <vtkRendererCollection.h>
#include <vtkRenderingOpenGLConfigure.h>
#include <vtkVersion.h>
#include <vtkWindowToImageFilter.h>
#include <vtkOpenGLRenderer.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkAxesActor.h>
#include <vtkMatrix4x4.h>
#include <vtkTransform.h>
#include <vtkBoundingBox.h>
#include <vtkPolyData.h>
#include <vtkGridAxesActor3D.h>

#ifdef VTK_USE_X
#include <vtkF3DGLXRenderWindow.h>
#endif

#ifdef _WIN32
#include <vtkF3DWGLRenderWindow.h>
#endif

#ifdef VTK_OPENGL_HAS_EGL
#include <vtkF3DEGLRenderWindow.h>
#endif

#if VTK_VERSION_NUMBER >= VTK_VERSION_CHECK(9, 3, 20240914)
#include <vtkOSOpenGLRenderWindow.h>
#endif

#include <sstream>

#include "settings.h"

#include <QDebug>

using namespace f3d;
using namespace f3d::detail;

window_impl::window_impl(DS::Settings* psettings, vtkRenderWindow* vtkwindow)
	: settings(psettings), RenWin(vtkwindow)
{
	this->RenWin->EnableTranslucentSurfaceOn();
	this->RenWin->SetMultiSamples(0); // Disable hardware antialiasing
	this->RenWin->SetOffScreenRendering(true);
	this->RenWin->AddRenderer(this->Renderer); 

	this->Camera = std::make_unique<detail::camera_impl>();
	this->Camera->SetVTKRenderer(this->Renderer);

	this->Initialize();

	qDebug() << "VTK window class type is " << this->RenWin->GetClassName();
}

void window_impl::Initialize()
{
	Renderer->AddActor(GridAxesActor);
	this->GridAxesActor->SetUseBounds(false);
	this->GridAxesActor->SetVisibility(false);
}

void window_impl::InitializeUpVector()
{
	//this->Renderer->InitializeUpVector(this->Options.scene.up_direction);
}

window_impl::~window_impl()
{
	// The axis widget should be disabled before calling the renderer destructor
	// As there is a register loop if not
	this->GridAxesActor->SetVisibility(false);
}

void window_impl::UpdateDynamicOptions(bool force)
{
  // Set the cache path if not already
  //renderer->SetCachePath(this->CachePath.string());

  // Make sure lights are created before we take options into account
  //renderer->UpdateLights();

	this->ShowAxes(settings->showAxes(), force);
	this->ShowGrid(settings->showGrid(), force);

	//renderer->SetBackground(opt.render.background.color.data());
	//renderer->SetUseBlurBackground(opt.render.background.blur.enable);
	//renderer->SetBlurCircleOfConfusionRadius(opt.render.background.blur.coc);
  
	//renderer->SetGridUnitSquare(opt.render.grid.unit);
	//renderer->SetGridSubdivisions(opt.render.grid.subdivisions);
	//renderer->SetGridAbsolute(opt.render.grid.absolute);
	//renderer->ShowGrid(opt.render.grid.enable);
	//renderer->SetGridColor(opt.render.grid.color);

	//if (!opt.scene.camera.index.has_value())
	//{
		///renderer->SetUseOrthographicProjection(opt.scene.camera.orthographic);
	//}

	//renderer->UpdateActors();
}
/*
bool window_impl::render()
{
	// bbb this->UpdateDynamicOptions();
	this->RenWin->Render();
	return true;
}
*/

void window_impl::ShowAxes(bool show, bool force)
{
	// Dynamic visible axis
	// XXX this could be handled in UpdateActors
	// but it is not needed as axis actor is not impacted by
	// by any other parameters and require special
	// care when destructing this renderer
	if (settings->showAxes() != show || force)
	{
		this->AxisWidget = nullptr;
		if (show)
		{
			vtkNew<vtkAxesActor> axes;
			this->AxisWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
			this->AxisWidget->SetOrientationMarker(axes);
			this->AxisWidget->SetInteractor(this->RenWin->GetInteractor());
			this->AxisWidget->On();
			this->AxisWidget->InteractiveOff();
			this->AxisWidget->SetKeyPressActivation(false);
			this->AxisWidget->SetViewport(0.0, 0.0, 0.2, 0.2);
		}
	}
	if (settings->showAxes() != show)
		settings->setShowAxes(show);
}

vtkBoundingBox window_impl::ComputeVisiblePropOrientedBounds(const vtkMatrix4x4* matrix)
{
	const auto isMatrixAxisAligned = [](const vtkMatrix4x4* m, const double tol = 1e-8)
		{
			for (size_t i = 0; i < 3; ++i)
			{
				size_t nonzerosI = 0;
				size_t nonzerosJ = 0;
				for (size_t j = 0; j < 3; ++j)
				{
					if (::abs(m->Element[i][j]) > tol)
					{
						nonzerosI++;
					}
					if (::abs(m->Element[j][i]) > tol)
					{
						nonzerosJ++;
					}
				}
				if (nonzerosI > 1 || nonzerosJ > 1)
				{
					return false;
				}
			}
			return true;
		};

	/* Use `PokeMatrix` around the call to `GetBounds()` to extend box.
	 * Only gives the thightest bounds if the transformation is axis-aligned. */
	const auto extendBoxAxisAligned = [&](vtkProp3D* prop3d, vtkBoundingBox& box)
		{
			vtkNew<vtkMatrix4x4> tmpMatrix;
			vtkMatrix4x4::Multiply4x4(matrix, prop3d->GetMatrix(), tmpMatrix);
			prop3d->PokeMatrix(tmpMatrix);

			box.AddBounds(prop3d->GetBounds());

			prop3d->PokeMatrix(nullptr);
		};

	/* Use custom logic to extend box.
	 * Should give the tightest bounds even when non-axis-aligned.
	 * Only implemented for `vtkPolyDataMapper`-based actors. */
	const auto extendBoxArbitrary = [&](vtkProp3D* prop3d, vtkBoundingBox& box)
		{
			vtkActor* actor = vtkActor::SafeDownCast(prop3d);
			if (actor)
			{
				vtkPolyDataMapper* polyMapper = vtkPolyDataMapper::SafeDownCast(actor->GetMapper());
				if (polyMapper)
				{
					vtkPolyData* polydata = polyMapper->GetInput();
					if (polydata)
					{
						vtkNew<vtkMatrix4x4> tmpMatrix;
						vtkMatrix4x4::Multiply4x4(matrix, actor->GetMatrix(), tmpMatrix);
						double p[4] = { 0, 0, 0, 1 };
						double q[4];
						for (vtkIdType i = 0; i < polydata->GetNumberOfPoints(); ++i)
						{
							polydata->GetPoint(i, p);
							tmpMatrix->MultiplyPoint(p, q);
							box.AddPoint(q);
						}
						return;
					}
				}
			}
			assert(false); /* custom bounds calculations not implemented for this `vtkProp3D` */
		};

	const bool isAxisAligned = isMatrixAxisAligned(matrix);
	vtkBoundingBox box;

	/* use `ComputeVisiblePropBounds()`'s logic to iterate `vtkProp3D`s contributing to the bounds */
	//vtkProp* prop;
	//vtkCollectionSimpleIterator pit;

	vtkActorCollection* Props = Renderer->GetActors();
	Props->InitTraversal();
	vtkActor* prop = nullptr;
	while ((prop = Props->GetNextActor()) != nullptr)
		// for (this->Props->InitTraversal(pit); (prop = this->Props->GetNextProp(pit));)
	{
		if (prop->GetVisibility() && prop->GetUseBounds())
		{
			const double* bounds = prop->GetBounds();
			if (bounds != nullptr && vtkMath::AreBoundsInitialized(bounds))
			{
				vtkProp3D* prop3d = vtkProp3D::SafeDownCast(prop);
				if (prop3d)
				{
					if (isAxisAligned)
					{
						extendBoxAxisAligned(prop3d, box);
					}
					else
					{
						extendBoxArbitrary(prop3d, box);
					}
				}
			}
		}
	}

	return box;
}

bool window_impl::ShowGrid(bool show, bool force)
{
	if (settings->showGrid() != show || force)
	{
		if (show)
		{
			double* up = Renderer->GetEnvironmentUp();
			double* right = Renderer->GetEnvironmentRight();
			double front[3];
			vtkMath::Cross(right, up, front);

			vtkNew<vtkMatrix4x4> upMatrix;
			const double m[16] = {
			  right[0], right[1], right[2], 0, //
			  up[0], up[1], up[2], 0,          //
			  front[0], front[1], front[2], 0, //
			  0, 0, 0, 1,                      //
			};
			upMatrix->DeepCopy(m);
			vtkNew<vtkMatrix4x4> upMatrixInv;
			upMatrixInv->DeepCopy(upMatrix);
			upMatrixInv->Transpose();

			double orientation[3];
			vtkTransform::GetOrientation(orientation, upMatrixInv);
			const vtkBoundingBox bbox = ComputeVisiblePropOrientedBounds(upMatrix);

			if (!bbox.IsValid())
			{
				return false;
			}
			else
			{
				this->GridAxesActor->SetOrientation(orientation);
				this->GridAxesActor->SetVisibility(true);

				double center[4] = { 0, 0, 0, 1 };
				bbox.GetCenter(center);

				this->GridAxesActor->SetPosition(center);

				double a, b, c, x, y, z;
				bbox.GetBounds(a, b, c, x, y, z);
				GridAxesActor->SetGridBounds(a, b, c, x, y, z);

				GridAxesActor->SetXTitle("X Axis");
				GridAxesActor->SetYTitle("Y Axis");
				GridAxesActor->SetZTitle("Z Axis");
			}
		}
		this->GridAxesActor->SetVisibility(show);
	}
	if (settings->showGrid() != show)
		settings->setShowGrid(show);
	return true;
}

/*
vtkSmartPointer<vtkRenderWindow> f3d::detail::window_impl::AutoBackendWindow()
{
	// Override VTK logic
#ifdef _WIN32
	return vtkSmartPointer<vtkF3DWGLRenderWindow>::New();
#elif defined(__linux__) || defined(__FreeBSD__)
#if defined(VTK_USE_X)
		// try GLX
	vtkSmartPointer<vtkRenderWindow> glxRenWin = vtkSmartPointer<vtkF3DGLXRenderWindow>::New();
	if (glxRenWin)
	{
		return glxRenWin;
	}
#endif
#if defined(VTK_OPENGL_HAS_EGL)
	// try EGL
	vtkSmartPointer<vtkRenderWindow> eglRenWin = vtkSmartPointer<vtkF3DEGLRenderWindow>::New();
	if (eglRenWin)
	{
		return eglRenWin;
	}
#endif
#if VTK_VERSION_NUMBER >= VTK_VERSION_CHECK(9, 3, 20240914)
	// OSMesa
	return vtkSmartPointer<vtkOSOpenGLRenderWindow>::New();
#endif
	return nullptr;
#else
		// fallback on VTK logic for other systems
	return vtkSmartPointer<vtkRenderWindow>::New();
#endif
}
*/
