#include "app.h"
#include "settings.h"
#include "manager.h"
#include "vtkitem.h"

#include <QThread>

#include <vtkDataAssembly.h>
#include <vtkProgressBarRepresentation.h>
#include <vtkAxesActor.h>

#include "vtkF3DGenericImporter.h"
#include "vtkF3DMetaImporter.h"

#include "reader_FBX.h"
#include "camera_impl.h"

namespace DS
{
vtkStandardNewMacro(VtkItem::Data)

void VtkItem::timerCall()
{
	dispatch_async([](vtkRenderWindow* renderWindow, vtkUserData userData) {
		if (renderWindow && renderWindow->GetInteractor())
			renderWindow->GetInteractor()->InvokeEvent(vtkCommand::InteractionEvent);
		});
	QThread::msleep(10);
}

VtkItem::vtkUserData VtkItem::initializeVTK(vtkRenderWindow* renderWindow)
{
	vtkNew<Data> vtk;
	vtk->_vtkItem = this;
    vtk->_renWin = renderWindow;

    vtk->_renderer = vtkSmartPointer<vtkOpenGLRenderer>::New();

    renderWindow->EnableTranslucentSurfaceOn();
    renderWindow->SetMultiSamples(0); // Disable hardware antialiasing
    renderWindow->SetOffScreenRendering(true);
    renderWindow->AddRenderer(vtk->_renderer);

	vtk->_camera = std::make_unique<f3d::detail::camera_impl>();
    vtk->_camera->SetVTKRenderer(vtk->_renderer);

    vtk->_importer = vtkSmartPointer<vtkF3DMetaImporter>::New();
    vtk->_importer->SetRenderWindow(renderWindow);

    vtk->_gridactor = vtkSmartPointer<vtkActor>::New();
    vtk->_renderer->AddActor(vtk->_gridactor);

    vtk->_axiswidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
    vtk->_gridmapper = vtkSmartPointer<vtkF3DOpenGLGridMapper>::New();

	vtk->_timercb = vtkSmartPointer<vtkCallbackCommand>::New();
	renderWindow->GetInteractor()->CreateRepeatingTimer(10);
	vtk->_timercb->SetCallback([](vtkObject*, unsigned long, void* clientData, void*) {
		Data* vtk = static_cast<Data*>(clientData);
		auto mgr = vtk->_vtkItem->_manager;
		if (mgr->_playing) {
			mgr->_curtime += 1.0/30;
			if (mgr->_curtime > mgr->_timerg[1])
				mgr->_curtime = mgr->_timerg[0];
			if (!vtk->_importer->UpdateAtTimeValue(mgr->_curtime))
				qDebug() << "Importer update error.";
            else {
                mgr->_sliderval = (mgr->_curtime - mgr->_timerg[0]) / (mgr->_timerg[1] - mgr->_timerg[0]);
                emit mgr->sliderValChanged();
            }
		}
		});
	renderWindow->GetInteractor()->AddObserver(vtkCommand::TimerEvent, vtk->_timercb);
	vtk->_timercb->SetClientData(vtk);

	vtk->_vtkItem->UpdateDynamicOptions(vtk);
	return vtk;
}

void VtkItem::destroyingVTK(vtkRenderWindow* renderWindow, vtkUserData userData)
{
	auto* vtk = Data::SafeDownCast(userData);

    vtk->_camera.reset();

    vtk->_gridactor->Delete();
    //vtk->_axiswidget->Delete();
    vtk->_gridmapper->Delete();

	vtk->_timercb->Delete();

    vtk->_importer->Delete();
    vtk->_renderer->Delete();
}

void VtkItem::openSource()
{
	dispatch_async([](vtkRenderWindow* renderWindow, vtkUserData userData) {
		Data* vtk = (Data*)userData.GetPointer();
		auto mgr = vtk->_vtkItem->_manager;
		mgr->_playing = false;
		if (renderWindow->IsCurrent())
		{
			vtk->_vtkItem->sceneClear(vtk);
			vtk->_vtkItem->sceneAdd(vtk, vtk->_vtkItem->_fname.toStdString());
			vtk->_camera->resetToBounds();
			vtk->_vtkItem->UpdateDynamicOptions(vtk);

			vtk->_vtkItem->setTreeView(vtk);
			vtk->_importer->EnableAnimation(0);
			int n;
			vtk->_importer->GetTemporalInformation(0, 30, n, mgr->_timerg, nullptr);
		}
		});
	QThread::msleep(10);
}

void VtkItem::setTreeView(Data* vtk)
{
	vtkF3DAssimpImporter* importer = reinterpret_cast<vtkF3DAssimpImporter*>(
		vtk->_importer->Pimpl->Importers[0].Importer.Get());
	_aiscene = importer->Internals->Scene;

	_manager->setTreeModel(importer);
}

void VtkItem::close()
{
	dispatch_async([](vtkRenderWindow* renderWindow, vtkUserData userData) {
		Data* vtk = (Data*)userData.GetPointer();
		vtk->_vtkItem->_manager->_playing = false;
		vtk->_vtkItem->sceneClear(vtk);
		renderWindow->Render();
		});
	QThread::msleep(10);
}

void VtkItem::play()
{
	dispatch_async([](vtkRenderWindow* renderWindow, vtkUserData userData) {
		Data* vtk = (Data*)userData.GetPointer();		
		vtk->_vtkItem->_manager->_playing = !vtk->_vtkItem->_manager->_playing;
		});	
	QThread::msleep(10);
}

void VtkItem::sliderMove()
{
	dispatch_async([](vtkRenderWindow* renderWindow, vtkUserData userData) {
		Data* vtk = (Data*)userData.GetPointer();
		auto mgr = vtk->_vtkItem->_manager;
		mgr->_playing = false;
		mgr->_curtime = mgr->_sliderval * (mgr->_timerg[1] - mgr->_timerg[0]);
		vtk->_importer->UpdateAtTimeValue(mgr->_curtime);
		});
	QThread::msleep(10);
}

void VtkItem::sceneAdd(VtkItem::Data* vtk, std::string fname)
{
	std::vector<vtkSmartPointer<vtkImporter>> importers;

    if (!vtksys::SystemTools::FileExists(fname, true)) {
	    qDebug() << fname + " does not exists";
		return;
	}
	reader_FBX* reader = new reader_FBX();
	if (!reader) {
	    qDebug() << "Unable to FBX reader.";
		return;
	}

	vtkSmartPointer<vtkImporter> importer = reader->createSceneReader(fname);
	if (!importer) {
	    // XXX: F3D Plugin CMake logic ensure there is either a scene reader or a geometry reader
		auto vtkReader = reader->createGeometryReader(fname);
		assert(vtkReader);
		vtkSmartPointer<vtkF3DGenericImporter> genericImporter =
			vtkSmartPointer<vtkF3DGenericImporter>::New();
		genericImporter->SetInternalReader(vtkReader);
		importer = genericImporter;
	}
	importers.emplace_back(importer);

	sceneLoad(vtk, importers);
}

void VtkItem::sceneClear(VtkItem::Data* vtk)
{
	// Clear the meta importer from all importers
	vtk->_importer->Clear();
	// Clear the window of all actors
	//this->Window->Initialize();
}

void VtkItem::sceneLoad(VtkItem::Data* vtk, const std::vector<vtkSmartPointer<vtkImporter>>& importers)
{
	for (const vtkSmartPointer<vtkImporter>& importer : importers)
		vtk->_importer->AddImporter(importer);

	//// this->Window.InitializeUpVector();

	// Update the meta importer, the will only update importers that have not been updated before
	if (!vtk->_importer->Update()) {
		vtk->_importer->Clear();
		///this->Window->Initialize();
	}
	// Initialize the animation using temporal information from the importer
	_manager->resetAnim();
}

/*
window_impl::window_impl(DS::Settings* psettings, vtkRenderWindow* vtkwindow)
    : settings(psettings), RenWin(vtkwindow)
{
    this->RenWin->EnableTranslucentSurfaceOn();
    this->RenWin->SetMultiSamples(0); // Disable hardware antialiasing
    this->RenWin->SetOffScreenRendering(true);
    this->RenWin->AddRenderer(this->Renderer);

    this->Camera = std::make_unique<detail::camera_impl>();
    this->Camera->SetVTKRenderer(this->Renderer);

    Renderer->AddActor(this->GridActor);

    qDebug() << "VTK window class type is " << this->RenWin->GetClassName();
}
*/
/*
void window_impl::InitializeUpVector()
{
    //this->Renderer->InitializeUpVector(this->Options.scene.up_direction);
}

window_impl::~window_impl()
{
    // The axis widget should be disabled before calling the renderer destructor
    // As there is a register loop if not
    // ??? this->GridActor->SetVisibility(false);
}
*/
void VtkItem::UpdateDynamicOptions(VtkItem::Data* vtk)
{
    ShowAxes(vtk, _manager->_settings->showAxes(), true);
    ShowGrid(vtk, _manager->_settings->showGrid(), true);
}

void VtkItem::ShowAxes(VtkItem::Data* vtk, bool show, bool force)
{
    // Dynamic visible axis
    // XXX this could be handled in UpdateActors
    // but it is not needed as axis actor is not impacted by
    // by any other parameters and require special
    // care when destructing this renderer
    if (_manager->_settings->showAxes() != show || force)
    {
        vtk->_axiswidget = nullptr;
        if (show)
        {
            vtkNew<vtkAxesActor> axes;
            vtk->_axiswidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
            vtk->_axiswidget->SetOrientationMarker(axes);
            vtk->_axiswidget->SetInteractor(vtk->_renWin->GetInteractor());
            vtk->_axiswidget->On();
            vtk->_axiswidget->InteractiveOff();
            vtk->_axiswidget->SetKeyPressActivation(false);
            vtk->_axiswidget->SetViewport(0.0, 0.0, 0.2, 0.2);
        }
    }
}

void VtkItem::ShowGrid(VtkItem::Data* vtk, bool show, bool force)
{
    if (_manager->_settings->showGrid() != show || force)
    {
        ConfigureGridUsingCurrentActors(vtk);
    }
}

void VtkItem::ConfigureGridUsingCurrentActors(VtkItem::Data* vtk)
{
    // Configure grid using visible prop bounds and actors
    // Also initialize GridInfo
    bool show = _manager->_settings->showGrid();
    if (show)
    {
        double* up = vtk->_renderer->GetEnvironmentUp();
        double* right = vtk->_renderer->GetEnvironmentRight();
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
        upMatrixInv->Transpose(); // matrix is orthonormal, no need to use `Invert()`

        const vtkBoundingBox bbox = this->ComputeVisiblePropOrientedBounds(vtk, upMatrix);

        if (!bbox.IsValid())
        {
            show = false;
        }
        else
        {
            double diag = bbox.GetDiagonalLength();

            double tmpUnitSquare;
            /* b
            if (this->GridUnitSquare.has_value())
            {
                tmpUnitSquare = this->GridUnitSquare.value();
            }
            else*/
            {
                tmpUnitSquare = pow(10.0, round(log10(diag * 0.1)));
            }

            double center[4] = { 0, 0, 0, 1 };
            bbox.GetCenter(center);

            double downShift = 0;
            /*
            if (this->GridAbsolute)
            {
                downShift += vtkMath::Dot(up, upMatrixInv->MultiplyDoublePoint(center));
            }
            else*/
            {
                // a small margin is added to the size to avoid z-fighting if large translucent
                // triangles are exactly aligned with the grid bounds
                constexpr double margin = 0.0001;
                downShift += bbox.GetLength(1) / 2 + margin;
            }

            double* gridPos = upMatrixInv->MultiplyDoublePoint(center);
            double delta[3];
            vtk->_renderer->GetEnvironmentUp(delta);
            vtkMath::MultiplyScalar(delta, downShift);
            vtkMath::Subtract(gridPos, delta, gridPos);
            /* b
            std::stringstream stream;
            stream << "Using grid unit square size = " << tmpUnitSquare << "\n"
                << "Grid origin set to [" << gridPos[0] << ", " << gridPos[1] << ", " << gridPos[2]
                << "]\n\n";
            this->GridInfo = stream.str();
            */
            vtk->_gridmapper->SetFadeDistance(diag);
            vtk->_gridmapper->SetUnitSquare(tmpUnitSquare);
            int GridSubdivisions = 10;
            vtk->_gridmapper->SetSubdivisions(GridSubdivisions);
            /*
            if (this->GridAbsolute)
            {
                gridMapper->SetOriginOffset(-center[0], -center[1], -center[2]);
            }*/

            double orientation[3];
            vtkTransform::GetOrientation(orientation, upMatrixInv);
            vtk->_gridactor->SetOrientation(orientation);
            vtk->_gridactor->SetPosition(gridPos);

            vtk->_gridactor->GetProperty()->SetColor(_gridcolor);
            vtk->_gridmapper->SetAxis1Color(::abs(right[0]), ::abs(right[1]), ::abs(right[2]), 1);
            vtk->_gridmapper->SetAxis2Color(::abs(front[0]), ::abs(front[1]), ::abs(front[2]), 1);

            vtk->_gridactor->ForceTranslucentOn();
            vtk->_gridactor->SetMapper(vtk->_gridmapper);
            vtk->_gridactor->UseBoundsOff();
            vtk->_gridactor->PickableOff();
            // b this->GridConfigured = true;
        }
    }

    vtk->_gridactor->SetVisibility(show);
    vtk->_renderer->ResetCameraClippingRange();
}

vtkBoundingBox VtkItem::ComputeVisiblePropOrientedBounds(VtkItem::Data* vtk, const vtkMatrix4x4* matrix)
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
    vtkProp* prop;
    vtkCollectionSimpleIterator pit;
    for (vtk->_renderer->GetViewProps()->InitTraversal(pit); (prop = vtk->_renderer->GetViewProps()->GetNextProp(pit));)
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

}
