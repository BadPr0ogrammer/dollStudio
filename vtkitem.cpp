#include "app.h"
#include "manager.h"
#include "vtkitem.h"

#include <QThread>

#include <vtkDataAssembly.h>
#include <vtkProgressBarRepresentation.h>

namespace DS
{
vtkStandardNewMacro(VtkItem::Data)

void VtkItem::setupOpt()
{
	_options.render.grid.enable = false;
}

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

	vtk->_win = new f3d::detail::window_impl(_options, f3d::window::Type::WGL, false, nullptr, renderWindow);
	vtk->_win->SetInteractor(renderWindow->GetInteractor());
	vtk->_scene = new f3d::detail::scene_impl(_options, *vtk->_win);
	vtk->_scene->SetInteractor(renderWindow->GetInteractor());

	vtk->_timercb = vtkSmartPointer<vtkCallbackCommand>::New();
	vtk->_win->Internals->Interactor->CreateRepeatingTimer(10);
	vtk->_timercb->SetCallback([](vtkObject*, unsigned long, void* clientData, void*) {
		Data* vtk = static_cast<Data*>(clientData);
		if (vtk->_scene->Internals->AnimationManager.IsPlaying()) {
			vtk->_scene->Internals->AnimationManager.Tick();
		}
		});
	vtk->_win->Internals->Interactor->AddObserver(vtkCommand::TimerEvent, vtk->_timercb);
	vtk->_timercb->SetClientData(vtk);

	vtk->_scene->Internals->AnimationManager.SetDeltaTime(1.0 / 30.0);
	vtk->_win->UpdateDynamicOptions();
	
	_animanager = &vtk->_scene->Internals->AnimationManager;
	return vtk;
}

void VtkItem::destroyingVTK(vtkRenderWindow* renderWindow, vtkUserData userData)
{
	_animanager = nullptr;
	_playf = false;
	auto* vtk = Data::SafeDownCast(userData);

	vtk->_scene->clear();
	vtk->_timercb->Delete();

	delete vtk->_win;
	vtk->_win = nullptr;
	delete vtk->_scene;
	vtk->_scene = nullptr;
}

bool VtkItem::openSource(bool clear)
{
	dispatch_async([&](vtkRenderWindow* renderWindow, vtkUserData userData) {
		bool ret = false;
		_playf = false;
		_animanager->StopAnimation();
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
	QThread::msleep(10);
	return false;
}

void VtkItem::setTreeView(Data* vtk, bool clear)
{
	vtkF3DAssimpImporter* importer = reinterpret_cast<vtkF3DAssimpImporter*>(
		vtk->_scene->Internals->MetaImporter->Pimpl->Importers[0].Importer.Get());
	_aiscene = importer->Internals->Scene;

	_manager->setTreeModel(importer, clear);
}

void VtkItem::close()
{
	dispatch_async([&](vtkRenderWindow* renderWindow, vtkUserData userData) {
		Data* vtk = (Data*)userData.GetPointer();
		_playf = false;
		_animanager->StopAnimation();
		vtk->_scene->clear();
		renderWindow->Render();
		});
	QThread::msleep(10);
}

void VtkItem::play()
{
	dispatch_async([&](vtkRenderWindow* renderWindow, vtkUserData userData) {
		Data* vtk = (Data*)userData.GetPointer();		
		_animanager->ToggleAnimation();
		_playf = _animanager->IsPlaying();
		});	
	QThread::msleep(10);
}

void VtkItem::sliderMove()
{
	dispatch_async([&](vtkRenderWindow* renderWindow, vtkUserData userData) {
		Data* vtk = (Data*)userData.GetPointer();
		if (_playf) {
			_playf = false;
			_animanager->StopAnimation();
		}
		double d = _animanager->TimeRange[1] - _animanager->TimeRange[0];
		_animanager->LoadAtTime(_manager->_sliderval * d);
		});
	QThread::msleep(10);
}

}
