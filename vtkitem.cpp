#include "app.h"
#include "manager.h"
#include "vtkitem.h"

#include <QThread>

#include <vtkDataAssembly.h>
#include <vtkProgressBarRepresentation.h>

#include "camera_impl.h"

namespace DS
{
vtkStandardNewMacro(VtkItem::Data)

void VtkItem::setupOpt()
{
	//_options.render.grid.enable = false;
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

	vtk->_win = new f3d::detail::window_impl(_manager->_settings, renderWindow);
	vtk->_scene = new f3d::detail::scene_impl(_manager, vtk->_win);

	vtk->_timercb = vtkSmartPointer<vtkCallbackCommand>::New();
	vtk->_win->RenWin->GetInteractor()->CreateRepeatingTimer(10);
	vtk->_timercb->SetCallback([](vtkObject*, unsigned long, void* clientData, void*) {
		Data* vtk = static_cast<Data*>(clientData);
		auto mgr = vtk->_vtkItem->_manager;
		if (mgr->_playing) {
			int n;
			double rg[2] = { 0,0 };
			vtk->_scene->MetaImporter->GetTemporalInformation(0, 30, n, rg, nullptr);
			mgr->_curtime += 1.0/30;
			if (mgr->_curtime > rg[1])
				mgr->_curtime = 0;
			if (!vtk->_scene->MetaImporter->UpdateAtTimeValue(mgr->_curtime))
				qDebug() << "Importer update error.";
		}
		});
	vtk->_win->RenWin->GetInteractor()->AddObserver(vtkCommand::TimerEvent, vtk->_timercb);
	vtk->_timercb->SetClientData(vtk);

	vtk->_win->UpdateDynamicOptions(true);

	return vtk;
}

void VtkItem::destroyingVTK(vtkRenderWindow* renderWindow, vtkUserData userData)
{
	auto* vtk = Data::SafeDownCast(userData);
	vtk->_vtkItem = nullptr;

	vtk->_scene->clear();
	vtk->_timercb->Delete();

	delete vtk->_win;
	vtk->_win = nullptr;
	delete vtk->_scene;
	vtk->_scene = nullptr;
}

void VtkItem::openSource()
{
	dispatch_async([](vtkRenderWindow* renderWindow, vtkUserData userData) {
		Data* vtk = (Data*)userData.GetPointer();
		vtk->_vtkItem->_manager->_playing = false;
		if (renderWindow->IsCurrent())
		{
			vtk->_scene->clear();
			vtk->_scene->add(vtk->_vtkItem->_fname.toStdString());
			vtk->_win->Camera->resetToBounds();
			vtk->_win->UpdateDynamicOptions(true);

			vtk->_vtkItem->setTreeView(vtk);
			vtk->_scene->MetaImporter->EnableAnimation(0);
		}
		});
	QThread::msleep(10);
}

void VtkItem::setTreeView(Data* vtk)
{
	vtkF3DAssimpImporter* importer = reinterpret_cast<vtkF3DAssimpImporter*>(
		vtk->_scene->MetaImporter->Pimpl->Importers[0].Importer.Get());
	_aiscene = importer->Internals->Scene;

	_manager->setTreeModel(importer);
}

void VtkItem::close()
{
	dispatch_async([](vtkRenderWindow* renderWindow, vtkUserData userData) {
		Data* vtk = (Data*)userData.GetPointer();
		vtk->_vtkItem->_manager->_playing = false;
		vtk->_scene->clear();
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
		double curtime = 0;
		{
			int n = -1;
			double rg[2] = { 0,0 };
			vtk->_scene->MetaImporter->GetTemporalInformation(0, 30, n, rg, nullptr);
			/////mgr->_curtick = mgr->_sliderval * n;
			//////////////curtime = mgr->_curtick  * (1.0 / 30) * (rg[1] - rg[0]) / n;
		}
		vtk->_scene->MetaImporter->UpdateAtTimeValue(curtime);
		});
	QThread::msleep(10);
}

}
