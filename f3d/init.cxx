#include "init.h"

#include "vtkF3DObjectFactory.h"

#if F3D_MODULE_EXR
#include "vtkF3DEXRReader.h"
#endif

#include <vtkImageReader2Factory.h>
#include <vtkLogger.h>
#include <vtkNew.h>
#include <vtkVersion.h>

#include <memory>

namespace f3d::detail
{

//----------------------------------------------------------------------------
void init::initialize()
{
  static std::unique_ptr<init> instance;
  if (!instance)
  {
    instance = std::make_unique<init>();
  }
}

//----------------------------------------------------------------------------
init::init()
{
  // Can be overridden by calling log::setVerboseLevel
  vtkObject::GlobalWarningDisplayOff();

  // Disable vtkLogger in case VTK was built with log support
  vtkLogger::SetStderrVerbosity(vtkLogger::VERBOSITY_OFF);
  vtkLogger::SetInternalVerbosityLevel(vtkLogger::VERBOSITY_OFF);

  // instantiate our own polydata mapper and output windows
  vtkNew<vtkF3DObjectFactory> factory;
  vtkObjectFactory::RegisterFactory(factory);
  vtkObjectFactory::SetAllEnableFlags(0, "vtkPolyDataMapper", "vtkOpenGLPolyDataMapper");

#if VTK_VERSION_NUMBER >= VTK_VERSION_CHECK(9, 3, 20240203)
  vtkObjectFactory::SetAllEnableFlags(0, "vtkPointGaussianMapper", "vtkOpenGLPointGaussianMapper");
#endif

#ifdef __EMSCRIPTEN__
  vtkObjectFactory::SetAllEnableFlags(0, "vtkRenderWindow", "vtkOpenGLRenderWindow");
  vtkObjectFactory::SetAllEnableFlags(
    0, "vtkRenderWindowInteractor", "vtkGenericRenderWindowInteractor");
#endif

#if F3D_MODULE_EXR
  vtkNew<vtkF3DEXRReader> reader;
  vtkImageReader2Factory::RegisterReader(reader);
#endif
}
}
