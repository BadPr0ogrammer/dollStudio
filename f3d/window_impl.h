/**
 * @class   window_impl
 * @brief   A concrete implementation of window
 *
 * A concrete implementation of window that can provide a window to render into.
 * It also defined implementation only API used by the libf3d.
 * See window.h for the class documentation
 */

#ifndef f3d_window_impl_h
#define f3d_window_impl_h

#include "context.h"
#include "window.h"

#include <filesystem>
#include <memory>
#include <optional>

class vtkRenderWindow;
class vtkF3DMetaImporter;
class vtkRenderWindowInteractor;

namespace f3d
{
class options;

namespace detail
{
class window_impl : public window
{
public:
  /**
   * Create the internal vtkRenderWindow using the offscreen param
   * and store option ref for later usage
   */
  window_impl(const options& options, const std::optional<Type>& type, bool offscreen,
    const context::function& getProcAddress, vtkRenderWindow *vtkwindow);
  /**
   * Default destructor
   */
  ~window_impl() override;

  ///@{
  /**
   * Documented public API
   */
  Type getType() override;
  bool isOffscreen() override;
  camera& getCamera() override;
  bool render() override;
  image renderToImage(bool noBackground = false) override;
  int getWidth() const override;
  int getHeight() const override;
  window& setSize(int width, int height) override;
  window& setPosition(int x, int y) override;
  window& setIcon(const unsigned char* icon, size_t iconSize) override;
  window& setWindowName(std::string_view windowName) override;
  point3_t getWorldFromDisplay(const point3_t& displayPoint) const override;
  point3_t getDisplayFromWorld(const point3_t& worldPoint) const override;
  ///@}

  /**
   * Implementation only API.
   * Initialize the renderer by clearing it of all actors.
   */
  void Initialize();

  /**
   * Implementation only API.
   * Initialize the up vector on the renderer using the Up string option
   */
  void InitializeUpVector();

  /**
   * Implementation only API.
   * Set the importer on the internal renderer
   */
  void SetImporter(vtkF3DMetaImporter* importer);

  /**
   * Implementation only API.
   * Use all the rendering related options to update the configuration of the window
   * and the rendering stack below.
   * This is called automatically when calling scene::add and window::render but can also be called
   * manually when needed. Return true on success, false otherwise.
   */
  void UpdateDynamicOptions();

  /**
   * Implementation only API.
   * Print scene description to log using provided verbose level
   */
  //b void PrintSceneDescription(log::VerboseLevel level);

  /**
   * Implementation only API.
   * Print coloring description to log using provided verbose level if available
   */
  //b void PrintColoringDescription(log::VerboseLevel level);

  /**
   * Implementation only API.
   * Get a pointer to the internal vtkRenderWindow
   */
  virtual vtkRenderWindow* GetRenderWindow();

  /**
   * Implementation only API.
   * Set the cache path.
   */
  void SetCachePath(const std::filesystem::path& cachePath);

  /**
   * Implementation only API.
   * Set the interactor to use when recovering bindings documentation.
   */
   //void SetInteractor(interactor_impl* interactor);
  void SetInteractor(vtkRenderWindowInteractor* interactor);

  /**
   * Trigger a render only of the UI
   * Does nothing if F3D_MODULE_UI is OFF
   */
  void RenderUIOnly();

// b private:
  class internals;
  std::unique_ptr<internals> Internals;
};

}
}

// b #include "engine.h"
#include "camera_impl.h"
#include "macros.h"
#include "options.h"
#include "utils.h"

#include "vtkF3DExternalRenderWindow.h"

#include "vtkF3DGenericImporter.h"
#include "vtkF3DNoRenderWindow.h"
#include "vtkF3DRenderer.h"

#include <vtkCamera.h>
#include <vtkF3DRenderPass.h>
#include <vtkImageData.h>
#include <vtkImageExport.h>
#include <vtkInformation.h>
#include <vtkPNGReader.h>
#include <vtkPointGaussianMapper.h>
#include <vtkRendererCollection.h>
#include <vtkRenderingOpenGLConfigure.h>
#include <vtkVersion.h>
#include <vtkWindowToImageFilter.h>

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


namespace fs = std::filesystem;

namespace f3d::detail
{
    class window_impl::internals
    {
    public:
        explicit internals(const options& options)
            : Options(options)
        {
        }

        static context::fptr SymbolLoader(void* userptr, const char* name)
        {
            assert(userptr != nullptr);
            auto* fn = static_cast<context::function*>(userptr);
            assert(fn != nullptr);
            return (*fn)(name);
        }

        static vtkSmartPointer<vtkRenderWindow> AutoBackendWindow()
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

        // b vtkSmartPointer<vtkRenderWindow> RenWin;
        // interactor_impl* Interactor = nullptr;
        vtkRenderWindow* RenWin = nullptr;
        vtkRenderWindowInteractor* Interactor = nullptr;
        
        std::unique_ptr<camera_impl> Camera;
        vtkNew<vtkF3DRenderer> Renderer;
        const options& Options;
        fs::path CachePath;
        context::function GetProcAddress;
    };
}

#endif
