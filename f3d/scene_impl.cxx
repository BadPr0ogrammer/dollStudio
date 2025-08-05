#include "scene_impl.h"

#include "animationManager.h"
#include "window_impl.h"
#include "factory.h"

#include "vtkF3DGenericImporter.h"
#include "vtkF3DMetaImporter.h"

#include <vtkCallbackCommand.h>
#include <vtkProgressBarRepresentation.h>
#include <vtkProgressBarWidget.h>
#include <vtkTimerLog.h>
#include <vtkVersion.h>
#include <vtksys/SystemTools.hxx>

#include <vtkRenderWindowInteractor.h>

#include <vector>

#include "settings.h"

#include <QDebug>

namespace fs = std::filesystem;
using namespace f3d::detail;

scene_impl::scene_impl(DS::Settings* psettings, window_impl* window)
	: settings(psettings), Window(window)
{
	AnimationManager = new animationManager(psettings, window);
	this->MetaImporter->SetRenderWindow(this->Window->RenWin);
	AnimationManager->SetImporter(MetaImporter);
}

scene_impl::~scene_impl()
{
	delete AnimationManager;
}

void scene_impl::add(const fs::path& filePath)
{
	std::vector<fs::path> paths = { filePath };
	this->add(paths);
}

void scene_impl::add(const std::vector<fs::path>& filePaths)
{
	if (filePaths.empty()) {
		qDebug() << "No file to load a full scene provided\n";
		return;
	}

	std::vector<vtkSmartPointer<vtkImporter>> importers;
	for (const fs::path& filePath : filePaths)
	{
		if (filePath.empty()) {
			qDebug() << "An empty file to load was provided\n";
			continue;
		}
		if (!vtksys::SystemTools::FileExists(filePath.string(), true)) {
			qDebug() << filePath.string() + " does not exists";
			return;
		}
		std::optional<std::string> forceReader;// = this->Internals->Options.scene.force_reader;
		// Recover the importer for the provided file path
		f3d::reader* reader = f3d::factory::instance()->getReader(filePath.string(), forceReader);
		if (!reader) {
			qDebug() << "Unable to reader.";
			return;
		}

		vtkSmartPointer<vtkImporter> importer = reader->createSceneReader(filePath.string());
		if (!importer) {
			// XXX: F3D Plugin CMake logic ensure there is either a scene reader or a geometry reader
			auto vtkReader = reader->createGeometryReader(filePath.string());
			assert(vtkReader);
			vtkSmartPointer<vtkF3DGenericImporter> genericImporter =
				vtkSmartPointer<vtkF3DGenericImporter>::New();
			genericImporter->SetInternalReader(vtkReader);
			importer = genericImporter;
		}
		importers.emplace_back(importer);
	}

	qDebug() << "\nLoading files: ";
	if (filePaths.size() == 1)
		qDebug() << filePaths[0].string();
	else for (const fs::path& filePathStr : filePaths)
		qDebug() << "- " << filePathStr.string();

	this->Load(importers);
}

void scene_impl::clear()
{
	// Clear the meta importer from all importers
	this->MetaImporter->Clear();
	// Clear the window of all actors
	this->Window->Initialize();
}

bool scene_impl::supports(const fs::path& filePath)
{
	std::optional<std::string> force_reader;
	return f3d::factory::instance()->getReader(
		filePath.string(), /*this->Internals->Options.scene.*/force_reader) != nullptr;
}

void scene_impl::Load(const std::vector<vtkSmartPointer<vtkImporter>>& importers)
{
	for (const vtkSmartPointer<vtkImporter>& importer : importers)
		this->MetaImporter->AddImporter(importer);

	//// this->Window.InitializeUpVector();

	// Update the meta importer, the will only update importers that have not been updated before
	if (!this->MetaImporter->Update()) {
		this->MetaImporter->Clear();
		this->Window->Initialize();
	}
	// Initialize the animation using temporal information from the importer
	this->AnimationManager->Initialize();
}

#if 0
namespace f3d
{
namespace detail
{
class scene_impl::internals
{
public:
	internals(DS::Settings* psettings, window_impl& window)
		: settings(psettings)
		, Window(window)
		, AnimationManager(psettings, window)
	{
		this->MetaImporter->SetRenderWindow(this->Window.RenWin);
		//this->Window.SetImporter(this->MetaImporter);
		this->AnimationManager.SetImporter(this->MetaImporter);
	}

	struct ProgressDataStruct
	{
		vtkTimerLog* timer;
		vtkProgressBarWidget* widget;
	};

	static void CreateProgressRepresentationAndCallback(
		ProgressDataStruct* data, vtkImporter* importer, vtkRenderWindowInteractor* interactor)//interactor_impl* interactor
	{
		vtkNew<vtkCallbackCommand> progressCallback;
		progressCallback->SetClientData(data);
		progressCallback->SetCallback(
			[](vtkObject*, unsigned long, void* clientData, void* callData)
			{
				auto progressData = static_cast<ProgressDataStruct*>(clientData);
				progressData->timer->StopTimer();
				vtkProgressBarWidget* widget = progressData->widget;
				// Only show and render the progress bar if loading takes more than 0.15 seconds
				if (progressData->timer->GetElapsedTime() > 0.15 ||
					vtksys::SystemTools::HasEnv("CTEST_F3D_PROGRESS_BAR"))
				{
					widget->On();
					vtkProgressBarRepresentation* rep =
						vtkProgressBarRepresentation::SafeDownCast(widget->GetRepresentation());
					rep->SetProgressRate(*static_cast<double*>(callData));
					widget->Render();
				}
			});
		importer->AddObserver(vtkCommand::ProgressEvent, progressCallback);

		// b interactor->SetInteractorOn(data->widget); ????

		vtkProgressBarRepresentation* progressRep =
			vtkProgressBarRepresentation::SafeDownCast(data->widget->GetRepresentation());
		progressRep->SetProgressRate(0.0);
		progressRep->ProportionalResizeOff();
		progressRep->SetPosition(0.0, 0.0);
		progressRep->SetPosition2(1.0, 0.0);
		progressRep->SetMinimumSize(0, 5);
		progressRep->SetProgressBarColor(1, 1, 1);
		progressRep->DrawBackgroundOff();
		progressRep->DragableOff();
		progressRep->SetShowBorderToOff();
		progressRep->DrawFrameOff();
		progressRep->SetPadding(0.0, 0.0);
		data->timer->StartTimer();
	}

#endif
