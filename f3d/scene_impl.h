/**
 * @class   scene_impl
 * @brief   A concrete implementation of scene
 *
 * A concrete implementation of scene that hides the private API
 * See scene.h for the class documentation
 */

#ifndef f3d_scene_impl_h
#define f3d_scene_impl_h

#include "scene.h"

#include <memory>

class vtkRenderWindowInteractor;

namespace f3d
{
class options;

namespace detail
{
//class interactor_impl;
class window_impl;
class scene_impl : public scene
{
public:
  ///@{
  /**
   * Documented public API
   */
  scene_impl(options& options, window_impl& window);
  ~scene_impl();
  scene& add(const std::filesystem::path& filePath) override;
  scene& add(const std::vector<std::filesystem::path>& filePath) override;
  scene& add(const std::vector<std::string>& filePathStrings) override;
  scene& add(const mesh_t& mesh) override;
  scene& clear() override;
  bool supports(const std::filesystem::path& filePath) override;
  scene& loadAnimationTime(double timeValue) override;
  std::pair<double, double> animationTimeRange() override;
  unsigned int availableAnimations() const override;
  ///@}

  /**
   * Implementation only API.
   * Set the interactor to use when interacting and set the AnimationManager on the interactor.
   */
  //void SetInteractor(interactor_impl* interactor);
  void SetInteractor(vtkRenderWindowInteractor* interactor);

  /**
   * Display available cameras in the log
   */
  //b void PrintImporterDescription(log::VerboseLevel level);

// b private:
  class internals;
  std::unique_ptr<internals> Internals;
};
}
}

#include "animationManager.h"
#include "options.h"
#include "scene.h"
#include "window_impl.h"

#include "factory.h"
#include "vtkF3DGenericImporter.h"
#include "vtkF3DMemoryMesh.h"
#include "vtkF3DMetaImporter.h"

#include <optional>
#include <vtkCallbackCommand.h>
#include <vtkProgressBarRepresentation.h>
#include <vtkProgressBarWidget.h>
#include <vtkTimerLog.h>
#include <vtkVersion.h>
#include <vtksys/SystemTools.hxx>

#include <vtkRenderWindowInteractor.h>

#include <vector>

namespace f3d
{
namespace detail
{
class scene_impl::internals
{
public:
    internals(options& options, window_impl& window)
        : Options(options)
        , Window(window)
        , AnimationManager(options, window)
    {
        this->MetaImporter->SetRenderWindow(this->Window.GetRenderWindow());
        this->Window.SetImporter(this->MetaImporter);
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

    void Load(const std::vector<vtkSmartPointer<vtkImporter>>& importers)
    {
        for (const vtkSmartPointer<vtkImporter>& importer : importers)
        {
            this->MetaImporter->AddImporter(importer);
        }

        // Initialize the UpVector on load
        this->Window.InitializeUpVector();

        // Reset temporary up to apply any config values
        if (this->Interactor)
        {
            // b this->Interactor->ResetTemporaryUp();
        }

        if (this->Options.scene.camera.index.has_value())
        {
            this->MetaImporter->SetCameraIndex(this->Options.scene.camera.index.value());
        }

        // Manage progress bar
        vtkNew<vtkProgressBarWidget> progressWidget;
        vtkNew<vtkTimerLog> timer;
        scene_impl::internals::ProgressDataStruct callbackData;
        callbackData.timer = timer;
        callbackData.widget = progressWidget;
        if (this->Options.ui.loader_progress && this->Interactor)
        {
            scene_impl::internals::CreateProgressRepresentationAndCallback(
                &callbackData, this->MetaImporter, this->Interactor);
        }

        // Update the meta importer, the will only update importers that have not been updated before
#if VTK_VERSION_NUMBER >= VTK_VERSION_CHECK(9, 3, 20240707)
        if (!this->MetaImporter->Update())
        {
            this->MetaImporter->RemoveObservers(vtkCommand::ProgressEvent);
            // b progressWidget->Off();
            progressWidget->On();

            this->MetaImporter->Clear();
            this->Window.Initialize();
            throw scene::load_failure_exception("failed to load scene");
        }
#else
        this->MetaImporter->Update();
#endif

        // Remove anything progress related if any
        this->MetaImporter->RemoveObservers(vtkCommand::ProgressEvent);
        progressWidget->Off();

        // Initialize the animation using temporal information from the importer
        this->AnimationManager.Initialize();

        // Update all window options and reset camera to bounds if needed
        this->Window.UpdateDynamicOptions();
        if (!this->Options.scene.camera.index.has_value())
        {
            this->Window.getCamera().resetToBounds();
        }

        // b scene_impl::internals::DisplayAllInfo(this->MetaImporter, this->Window);
    }
/* b
    static void DisplayImporterDescription(log::VerboseLevel level, vtkImporter* importer)
    {
        vtkIdType availCameras = importer->GetNumberOfCameras();
        if (availCameras <= 0)
        {
            log::print(level, "No camera available");
        }
        else
        {
            log::print(level, "Camera(s) available are:");
        }
        for (int i = 0; i < availCameras; i++)
        {
            log::print(level, i, ": ", importer->GetCameraName(i));
        }
        log::print(level, "");
        log::print(level, importer->GetOutputsDescription(), "\n");
    }

    static void DisplayAllInfo(vtkImporter* importer, window_impl& window)
    {
        // Display output description
        scene_impl::internals::DisplayImporterDescription(log::VerboseLevel::DEBUG, importer);

        // Display coloring information
        window.PrintColoringDescription(log::VerboseLevel::DEBUG);
        log::debug("");

        // Print scene description
        window.PrintSceneDescription(log::VerboseLevel::DEBUG);
    }
*/
    const options& Options;
    window_impl& Window;
    //interactor_impl* Interactor = nullptr;
    vtkRenderWindowInteractor* Interactor = nullptr;
    animationManager AnimationManager;

    vtkNew<vtkF3DMetaImporter> MetaImporter;
};
}
}
#endif
