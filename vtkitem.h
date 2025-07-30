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
#include <vtkRenderWindowInteractor.h>
#include <vtkCallbackCommand.h>
#include <vtkRenderTimerLog.h>
#include <vtkProgressBarWidget.h>

#include "vtkF3DMetaImporter.h"
#include "vtkF3DRenderer.h"
#include "vtkF3DAssimpImporter.h"

#include "animationManager.h"
#include "window_impl.h"
#include "scene_impl.h"
#include "options.h"

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

		f3d::detail::window_impl* _win = nullptr;
		f3d::detail::scene_impl* _scene = nullptr;

		vtkSmartPointer<vtkCallbackCommand> _timercb;
	};
	QString _fname;
	f3d::options _options;

	Manager*						_manager = nullptr;
    bool							_playf = false;
	f3d::detail::animationManager*	_animanager = nullptr;
	const aiScene*					_aiscene = nullptr;

	vtkUserData initializeVTK(vtkRenderWindow* renderWindow) override;
	void destroyingVTK(vtkRenderWindow* renderWindow, vtkUserData userData) override;

	bool openSource(bool clear);
	void close();
	void play();
	void setupOpt();
	void setTreeView(Data* vtk, bool clear);
	void traversTree(QStandardItem* parent, const aiNode* node);
	void timerCall();
	void sliderMove();
};
}
