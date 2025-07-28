#include "app.h"
#include "manager.h"
#include "vtkitem.h"

#include <vtkDataAssembly.h>
#include <vtkProgressBarRepresentation.h>

namespace DS
{
vtkStandardNewMacro(VtkItem::Data)

void VtkItem::setupOpt()
{
	_options.render.grid.enable = false;

	connect(&_timer2, &QTimer::timeout, this, &VtkItem::timerSlot);
	_timer2.start();
}

void VtkItem::timerSlot()
{
	dispatch_async([](vtkRenderWindow* renderWindow, vtkUserData userData) {
		if (renderWindow && renderWindow->GetInteractor())
			renderWindow->GetInteractor()->InvokeEvent(vtkCommand::InteractionEvent);
		});
}

VtkItem::vtkUserData VtkItem::initializeVTK(vtkRenderWindow* renderWindow)
{
	vtkNew<Data> vtk;
	vtk->_vtkItem = this;

	vtk->_win = new f3d::detail::window_impl(_options, f3d::window::Type::WGL, false, nullptr, renderWindow);
	vtk->_win->SetInteractor(renderWindow->GetInteractor());
	vtk->_scene = new f3d::detail::scene_impl(_options, *vtk->_win);
	vtk->_scene->SetInteractor(renderWindow->GetInteractor());

	vtk->_timer1 = vtkSmartPointer<vtkCallbackCommand>::New();
	vtk->_win->Internals->Interactor->CreateRepeatingTimer(10);
	vtk->_timer1->SetCallback([](vtkObject*, unsigned long, void* clientData, void*) {
		Data* vtk = static_cast<Data*>(clientData);
		if (vtk->_scene->Internals->AnimationManager.IsPlaying()) {
			vtk->_scene->Internals->AnimationManager.Tick();
		}
		});
	vtk->_win->Internals->Interactor->AddObserver(vtkCommand::TimerEvent, vtk->_timer1);
	vtk->_timer1->SetClientData(vtk);

	vtk->_scene->Internals->AnimationManager.SetDeltaTime(1.0 / 30.0);
	vtk->_win->UpdateDynamicOptions();

	return vtk;
}

void VtkItem::destroyingVTK(vtkRenderWindow* renderWindow, vtkUserData userData)
{
	auto* vtk = Data::SafeDownCast(userData);

	vtk->_scene->clear();
	vtk->_timer1->Delete();

	delete vtk->_win;
	vtk->_win = nullptr;
	delete vtk->_scene;
	vtk->_scene = nullptr;
}

bool VtkItem::openSource(bool clear)
{
	_timer2.stop();
	dispatch_async([&](vtkRenderWindow* renderWindow, vtkUserData userData) {
		bool ret = false;
		Data* vtk = (Data*)userData.GetPointer();
		if (renderWindow->IsCurrent())
		{
			if (clear)
				vtk->_scene->clear();
			vtk->_scene->add(vtk->_vtkItem->_fname.toStdString());
			vtk->_win->getCamera().resetToBounds();
			ret = vtk->_win->render();

			setTreeView(vtk, clear);
		}
		return ret;
		});
	return false;
}

void VtkItem::setTreeView(Data* vtk, bool clear)
{
	vtkF3DAssimpImporter* importer = reinterpret_cast<vtkF3DAssimpImporter*>(
		vtk->_scene->Internals->MetaImporter->Pimpl->Importers[0].Importer.Get());
	_manager->setTreeModel(importer, clear);
}

void VtkItem::close()
{
	_timer2.stop();
	dispatch_async([](vtkRenderWindow* renderWindow, vtkUserData userData) {
		Data* vtk = (Data*)userData.GetPointer();
		vtk->_scene->clear();
		renderWindow->Render();
		});
}

void VtkItem::play()
{
	_timer2.start();
	dispatch_async([](vtkRenderWindow* renderWindow, vtkUserData userData) {
		Data* vtk = (Data*)userData.GetPointer();
		vtk->_scene->Internals->AnimationManager.ToggleAnimation();
		});
}
}
