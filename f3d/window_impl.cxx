#include "window_impl.h"

#include <QDebug>

using namespace f3d;
using namespace f3d::detail;

//----------------------------------------------------------------------------
window_impl::window_impl(DS::Settings* psettings, const std::optional<Type>& type, bool offscreen,
	const context::function& getProcAddress, vtkRenderWindow* vtkwindow)
	: Internals(std::make_unique<window_impl::internals>(psettings))
{
	this->Internals->GetProcAddress = getProcAddress;
	/* b
  if (type == Type::NONE)
  {
	this->Internals->RenWin = vtkSmartPointer<vtkF3DNoRenderWindow>::New();
  }
  else if (type == Type::EXTERNAL)
  {
	this->Internals->RenWin = vtkSmartPointer<vtkF3DExternalRenderWindow>::New();
  }
  else */
	if (type == Type::EGL)
	{
#if defined(VTK_OPENGL_HAS_EGL)
		this->Internals->RenWin = vtkSmartPointer<vtkF3DEGLRenderWindow>::New();
#else
		assert(false); // Unreachable
#endif
	}
	else if (type == Type::OSMESA)
	{
#if VTK_VERSION_NUMBER >= VTK_VERSION_CHECK(9, 3, 20240914)
		this->Internals->RenWin = vtkSmartPointer<vtkOSOpenGLRenderWindow>::New();
#else
		throw engine::no_window_exception(
			"Window type is OSMesa but the underlying VTK version is not recent enough to support it");
#endif
	}
	else if (type == Type::GLX)
	{
#if defined(VTK_USE_X)
		this->Internals->RenWin = vtkSmartPointer<vtkF3DGLXRenderWindow>::New();
#else
		assert(false); // Unreachable
#endif
	}
	else if (type == Type::WGL)
	{
#ifdef _WIN32
		this->Internals->RenWin = vtkwindow; // b vtkSmartPointer<vtkF3DWGLRenderWindow>::New();
#else
		assert(false); // Unreachable
#endif
	}
	else if (!type.has_value())
	{
		this->Internals->RenWin = internals::AutoBackendWindow();
	}

	// COCOA and WASM are not handled explicitly
	// as there is no helper method to create them in engine
	if (this->Internals->RenWin == nullptr)
	{
		// b throw engine::no_window_exception("Cannot create a window");
	}

#if VTK_VERSION_NUMBER >= VTK_VERSION_CHECK(9, 3, 20240914)
	vtkOpenGLRenderWindow* oglRenWin = vtkOpenGLRenderWindow::SafeDownCast(this->Internals->RenWin);
	if (oglRenWin)
	{
		if (this->Internals->GetProcAddress)
		{
			oglRenWin->SetOpenGLSymbolLoader(&internals::SymbolLoader, &this->Internals->GetProcAddress);
		}
	}
#endif

#if VTK_VERSION_NUMBER >= VTK_VERSION_CHECK(9, 3, 20240606)
	this->Internals->RenWin->EnableTranslucentSurfaceOn();
#endif
	this->Internals->RenWin->SetMultiSamples(0); // Disable hardware antialiasing
	this->Internals->RenWin->SetOffScreenRendering(offscreen);
	this->Internals->RenWin->SetWindowName("f3d");
	this->Internals->RenWin->AddRenderer(this->Internals->Renderer);
	this->Internals->Camera = std::make_unique<detail::camera_impl>();
	this->Internals->Camera->SetVTKRenderer(this->Internals->Renderer);

	//this->Internals->Renderer->SetConsoleBadgeEnabled(!offscreen
	// b || utils::getEnv("CTEST_F3D_CONSOLE_BADGE").has_value()
	//);

	this->Initialize();

	qDebug() << "VTK window class type is " << this->Internals->RenWin->GetClassName();

	Internals->Renderer->AddActor(Internals->GridAxesActor);
	this->Internals->GridAxesActor->SetVisibility(false);
}

//----------------------------------------------------------------------------
void window_impl::Initialize()
{
	//this->Internals->Renderer->Initialize();
}

//----------------------------------------------------------------------------
void window_impl::InitializeUpVector()
{
	//this->Internals->Renderer->InitializeUpVector(this->Internals->Options.scene.up_direction);
}

//----------------------------------------------------------------------------
window_impl::Type window_impl::getType()
{
	if (this->Internals->RenWin->IsA("vtkOSOpenGLRenderWindow"))
	{
		return Type::OSMESA;
	}

#ifdef VTK_USE_X
	if (this->Internals->RenWin->IsA("vtkF3DGLXRenderWindow"))
	{
		return Type::GLX;
	}
#endif

#ifdef _WIN32
	if (this->Internals->RenWin->IsA("vtkF3DWGLRenderWindow"))
	{
		return Type::WGL;
	}
#endif

#ifdef __APPLE__
	if (this->Internals->RenWin->IsA("vtkCocoaRenderWindow"))
	{
		return Type::COCOA;
	}
#endif

#ifdef VTK_OPENGL_HAS_EGL
	if (this->Internals->RenWin->IsA("vtkF3DEGLRenderWindow"))
	{
		return Type::EGL;
	}
#endif
#ifdef __EMSCRIPTEN__
	if (this->Internals->RenWin->IsA("vtkWebAssemblyOpenGLRenderWindow"))
	{
		return Type::WASM;
	}
#endif

	if (this->Internals->RenWin->IsA("vtkF3DNoRenderWindow"))
	{
		return Type::NONE;
	}

	return Type::UNKNOWN;
}

//----------------------------------------------------------------------------
bool window_impl::isOffscreen()
{
	return !this->Internals->RenWin->GetShowWindow();
}

//----------------------------------------------------------------------------

camera& window_impl::getCamera()
{
	return *this->Internals->Camera;
}

//----------------------------------------------------------------------------
int window_impl::getWidth() const
{
	return this->Internals->RenWin->GetSize()[0];
}

//----------------------------------------------------------------------------
int window_impl::getHeight() const
{
	return this->Internals->RenWin->GetSize()[1];
}

//----------------------------------------------------------------------------
window& window_impl::setSize(int width, int height)
{
	this->Internals->RenWin->SetSize(width, height);
	return *this;
}

//----------------------------------------------------------------------------
window& window_impl::setPosition(int x, int y)
{
	if (this->Internals->RenWin->IsA("vtkCocoaRenderWindow"))
	{
		// vtkCocoaRenderWindow has a different behavior than other render windows
		// https://gitlab.kitware.com/vtk/vtk/-/issues/18681
		int* screenSize = this->Internals->RenWin->GetScreenSize();
		int* winSize = this->Internals->RenWin->GetSize();
		this->Internals->RenWin->SetPosition(x, screenSize[1] - winSize[1] - y);
	}
	else
	{
		this->Internals->RenWin->SetPosition(x, y);
	}
	return *this;
}

//----------------------------------------------------------------------------
window& window_impl::setIcon(const unsigned char* icon, size_t iconSize)
{
	// XXX This code requires that the interactor has already been set on the render window
	vtkNew<vtkPNGReader> iconReader;
	iconReader->SetMemoryBuffer(icon);
	iconReader->SetMemoryBufferLength(iconSize);
	iconReader->Update();
	this->Internals->RenWin->SetIcon(iconReader->GetOutput());
	return *this;
}

//----------------------------------------------------------------------------
window& window_impl::setWindowName(std::string_view windowName)
{
	this->Internals->RenWin->SetWindowName(windowName.data());
	return *this;
}

//----------------------------------------------------------------------------
point3_t window_impl::getWorldFromDisplay(const point3_t& displayPoint) const
{
	point3_t out = { 0.0, 0.0, 0.0 };
	double worldPt[4];
	this->Internals->Renderer->SetDisplayPoint(displayPoint.data());
	this->Internals->Renderer->DisplayToWorld();
	this->Internals->Renderer->GetWorldPoint(worldPt);

	constexpr double homogeneousThreshold = 1e-7;
	if (worldPt[3] > homogeneousThreshold)
	{
		out[0] = worldPt[0] / worldPt[3];
		out[1] = worldPt[1] / worldPt[3];
		out[2] = worldPt[2] / worldPt[3];
	}
	return out;
}

//----------------------------------------------------------------------------
point3_t window_impl::getDisplayFromWorld(const point3_t& worldPoint) const
{
	point3_t out;
	this->Internals->Renderer->SetWorldPoint(worldPoint[0], worldPoint[1], worldPoint[2], 1.0);
	this->Internals->Renderer->WorldToDisplay();
	this->Internals->Renderer->GetDisplayPoint(out.data());
	return out;
}

//----------------------------------------------------------------------------
window_impl::~window_impl()
{
	if (this->Internals->Interactor)
	{
		// The axis widget should be disabled before calling the renderer destructor
		// As there is a register loop if not
		//  b this->Internals->Renderer->ShowAxis(false);
	}
}

//----------------------------------------------------------------------------
/* b
void window_impl::UpdateDynamicOptions()
{
  vtkF3DRenderer* renderer = this->Internals->Renderer;

  if (this->Internals->RenWin->IsA("vtkF3DNoRenderWindow"))
  {
	// With a NONE window type, only update the actors to get accurate bounding box information
	renderer->UpdateActors();
	return;
  }

  // Set the cache path if not already
  renderer->SetCachePath(this->Internals->CachePath.string());

  // Make sure lights are created before we take options into account
  renderer->UpdateLights();

  const options& opt = this->Internals->Options;

  if (this->Internals->Interactor)
  {
	renderer->ShowAxis(opt.ui.axis);
	renderer->SetUseTrackball(opt.interactor.trackball);
	renderer->SetInvertZoom(opt.interactor.invert_zoom);
  }

  // XXX: model.point_sprites.type only has an effect on geometry scene
  // but we set it here for practical reasons
  const int pointSpritesSize = opt.model.point_sprites.size;
  const vtkF3DRenderer::SplatType splatType = opt.model.point_sprites.type == "gaussian"
	? vtkF3DRenderer::SplatType::GAUSSIAN
	: vtkF3DRenderer::SplatType::SPHERE;
  renderer->SetPointSpritesProperties(splatType, pointSpritesSize);

  renderer->SetLineWidth(opt.render.line_width);
  renderer->SetPointSize(opt.render.point_size);
  renderer->ShowEdge(opt.render.show_edges);
  renderer->ShowTimer(opt.ui.fps);
  renderer->ShowFilename(opt.ui.filename);
  renderer->SetFilenameInfo(opt.ui.filename_info);
  renderer->ShowMetaData(opt.ui.metadata);
  renderer->ShowCheatSheet(opt.ui.cheatsheet);
  renderer->ShowConsole(opt.ui.console);
  renderer->ShowMinimalConsole(opt.ui.minimal_console);
  renderer->ShowDropZone(opt.ui.dropzone);
  renderer->SetDropZoneInfo(opt.ui.dropzone_info);
  renderer->ShowArmature(opt.render.armature.enable);

  renderer->SetUseRaytracing(opt.render.raytracing.enable);
  renderer->SetRaytracingSamples(opt.render.raytracing.samples);
  renderer->SetUseRaytracingDenoiser(opt.render.raytracing.denoise);

  vtkF3DRenderer::AntiAliasingMode aaMode = vtkF3DRenderer::AntiAliasingMode::NONE;

  // F3D_DEPRECATED
  // Remove this in the next major release
  F3D_SILENT_WARNING_PUSH()
  F3D_SILENT_WARNING_DECL(4996, "deprecated-declarations")
  if (opt.render.effect.anti_aliasing)
  {
	qDebug() << "render.effect.anti_aliasing is deprecated, please use " << "render.effect.antialiasing.enable instead";
	aaMode = vtkF3DRenderer::AntiAliasingMode::FXAA;
  }
  F3D_SILENT_WARNING_POP()

  if (opt.render.effect.antialiasing.enable)
  {
	if (opt.render.effect.antialiasing.mode == "fxaa")
	{
	  aaMode = vtkF3DRenderer::AntiAliasingMode::FXAA;
	}
	else if (opt.render.effect.antialiasing.mode == "ssaa")
	{
	  aaMode = vtkF3DRenderer::AntiAliasingMode::SSAA;
	}
	else
	{
	  qDebug() << opt.render.effect.antialiasing.mode << "is an invalid antialiasing mode. Valid modes are: fxaa, ssaa";
	}
  }

  renderer->SetUseSSAOPass(opt.render.effect.ambient_occlusion);
  renderer->SetAntiAliasingMode(aaMode);
  renderer->SetUseToneMappingPass(opt.render.effect.tone_mapping);
  renderer->SetUseDepthPeelingPass(opt.render.effect.translucency_support);
  renderer->SetBackfaceType(opt.render.backface_type);
  renderer->SetFinalShader(opt.render.effect.final_shader);

  renderer->SetBackground(opt.render.background.color.data());
  renderer->SetUseBlurBackground(opt.render.background.blur.enable);
  renderer->SetBlurCircleOfConfusionRadius(opt.render.background.blur.coc);
  renderer->SetLightIntensity(opt.render.light.intensity);

  renderer->SetHDRIFile(opt.render.hdri.file);
  renderer->SetUseImageBasedLighting(opt.render.hdri.ambient);
  renderer->ShowHDRISkybox(opt.render.background.skybox);

  renderer->SetFontFile(opt.ui.font_file);
  renderer->SetFontScale(opt.ui.scale);

  renderer->SetGridUnitSquare(opt.render.grid.unit);
  renderer->SetGridSubdivisions(opt.render.grid.subdivisions);
  renderer->SetGridAbsolute(opt.render.grid.absolute);
  renderer->ShowGrid(opt.render.grid.enable);
  renderer->SetGridColor(opt.render.grid.color);

  renderer->ShowAxesGrid(opt.render.axes_grid.enable);

  if (!opt.scene.camera.index.has_value())
  {
	renderer->SetUseOrthographicProjection(opt.scene.camera.orthographic);
  }

  renderer->SetSurfaceColor(opt.model.color.rgb);
  renderer->SetOpacity(opt.model.color.opacity);
  renderer->SetTextureBaseColor(opt.model.color.texture);
  renderer->SetTexturesTransform(opt.model.textures_transform);
  renderer->SetRoughness(opt.model.material.roughness);
  renderer->SetMetallic(opt.model.material.metallic);
  renderer->SetBaseIOR(opt.model.material.base_ior);
  renderer->SetTextureMaterial(opt.model.material.texture);
  renderer->SetTextureEmissive(opt.model.emissive.texture);
  renderer->SetEmissiveFactor(opt.model.emissive.factor);
  renderer->SetTextureNormal(opt.model.normal.texture);
  renderer->SetNormalScale(opt.model.normal.scale);
  renderer->SetTextureMatCap(opt.model.matcap.texture);

  renderer->SetEnableColoring(opt.model.scivis.enable);
  renderer->SetUseCellColoring(opt.model.scivis.cells);
  renderer->SetArrayNameForColoring(opt.model.scivis.array_name);
  renderer->SetComponentForColoring(opt.model.scivis.component);

  renderer->SetScalarBarRange(opt.model.scivis.range);
  renderer->SetColormap(opt.model.scivis.colormap);
  renderer->SetColormapDiscretization(opt.model.scivis.discretization);
  renderer->ShowScalarBar(opt.ui.scalar_bar);

  renderer->SetUsePointSprites(opt.model.point_sprites.enable);
  renderer->SetUseVolume(opt.model.volume.enable);
  renderer->SetUseInverseOpacityFunction(opt.model.volume.inverse);

  renderer->UpdateActors();

  // Update the cheatsheet if needed

  if (this->Internals->Interactor && renderer->CheatSheetNeedsUpdate())
  {
	std::vector<vtkF3DUIActor::CheatSheetGroup> cheatsheet;
	for (const std::string& group : this->Internals->Interactor->getBindGroups())
	{
	  std::vector<vtkF3DUIActor::CheatSheetTuple> groupList;
	  for (const interaction_bind_t& bind : this->Internals->Interactor->getBindsForGroup(group))
	  {
		auto [doc, val] = this->Internals->Interactor->getBindingDocumentation(bind);
		if (!doc.empty())
		{
		  groupList.emplace_back(std::make_tuple(bind.format(), doc, val));
		}
	  }
	  cheatsheet.emplace_back(std::make_pair(group, std::move(groupList)));
	}
	renderer->ConfigureCheatSheet(cheatsheet);
  }
}
*/

/*b
//----------------------------------------------------------------------------
void window_impl::PrintSceneDescription(log::VerboseLevel level)
{
  log::print(level, this->Internals->Renderer->GetSceneDescription());
}

//----------------------------------------------------------------------------
void window_impl::PrintColoringDescription(log::VerboseLevel level)
{
  std::string descr = this->Internals->Renderer->GetColoringDescription();
  if (!descr.empty())
  {
	log::print(level, descr);
  }
}
*/
//----------------------------------------------------------------------------
vtkRenderWindow* window_impl::GetRenderWindow()
{
	return this->Internals->RenWin;
}

//----------------------------------------------------------------------------
bool window_impl::render()
{
	// bbb this->UpdateDynamicOptions();
	this->Internals->RenWin->Render();
	return true;
}

//----------------------------------------------------------------------------
image window_impl::renderToImage(bool noBackground)
{
	this->render();

	vtkNew<vtkWindowToImageFilter> rtW2if;
	rtW2if->SetInput(this->Internals->RenWin);

	if (noBackground)
	{
		// we need to set the background to black to avoid blending issues with translucent
		// objects when saving to file with no background
		this->Internals->Renderer->SetBackground(0, 0, 0);
		rtW2if->SetInputBufferTypeToRGBA();
	}

	vtkNew<vtkImageExport> exporter;
	exporter->SetInputConnection(rtW2if->GetOutputPort());
	exporter->ImageLowerLeftOn();

	int* dims = exporter->GetDataDimensions();
	int cmp = exporter->GetDataNumberOfScalarComponents();

	image output(dims[0], dims[1], cmp);
	exporter->Export(output.getContent());

	return output;
}

//----------------------------------------------------------------------------
void window_impl::SetImporter(vtkF3DMetaImporter* importer)
{
	// b this->Internals->Renderer->SetImporter(importer);
}

//----------------------------------------------------------------------------
void window_impl::SetCachePath(const fs::path& cachePath)
{
	try
	{
		if (cachePath.empty())
		{
			// b throw engine::cache_exception("Provided cache path is empty");
		}

		// create directories if they do not exist
		fs::create_directories(cachePath);
	}
	catch (const fs::filesystem_error& ex)
	{
		// b throw engine::cache_exception(std::string("Could not use cache: ") + ex.what());
	}

	this->Internals->CachePath = cachePath;
}

//----------------------------------------------------------------------------
//void window_impl::SetInteractor(interactor_impl* interactor)
void window_impl::SetInteractor(vtkRenderWindowInteractor* interactor)
{
	this->Internals->Interactor = interactor;
}

//----------------------------------------------------------------------------
void window_impl::RenderUIOnly()
{
#if F3D_MODULE_UI
	// Do only a partial render of the UI
	vtkRenderWindow* renWin = this->Internals->RenWin;
	vtkRenderer* ren = renWin->GetRenderers()->GetFirstRenderer();
	vtkInformation* info = ren->GetInformation();

	info->Set(vtkF3DRenderPass::RENDER_UI_ONLY(), 1);
	renWin->Render();
	info->Remove(vtkF3DRenderPass::RENDER_UI_ONLY());
#endif
}

void window_impl::ShowAxis(bool show)
{
	// Dynamic visible axis
	// XXX this could be handled in UpdateActors
	// but it is not needed as axis actor is not impacted by
	// by any other parameters and require special
	// care when destructing this renderer
	if (Internals->settings->showAxis() != show)//this->AxisVisible != show)
	{
		this->Internals->AxisWidget = nullptr;
		if (show)
		{
			assert(this->Internals->Interactor);
			vtkNew<vtkAxesActor> axes;
#if VTK_VERSION_NUMBER >= VTK_VERSION_CHECK(9, 2, 20220907)
			this->Internals->AxisWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
#else
			this->AxisWidget = vtkSmartPointer<vtkF3DOrientationMarkerWidget>::New();
#endif
			this->Internals->AxisWidget->SetOrientationMarker(axes);
			this->Internals->AxisWidget->SetInteractor(this->Internals->Interactor);
			this->Internals->AxisWidget->On();
#if VTK_VERSION_NUMBER >= VTK_VERSION_CHECK(9, 2, 20220907)
			this->Internals->AxisWidget->InteractiveOff();
#endif
			this->Internals->AxisWidget->SetKeyPressActivation(false);
			this->Internals->AxisWidget->SetViewport(0.0, 0.0, 0.2, 0.2);
		}

		this->Internals->settings->setShowAxis(show);
	}
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

	vtkActorCollection* Props = Internals->Renderer->GetActors();
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

bool window_impl::ShowGrid(bool show)
{
	//bool show = this->AxesGridVisible;
	if (show)
	{
		double* up = Internals->Renderer->GetEnvironmentUp();
		double* right = Internals->Renderer->GetEnvironmentRight();
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
			this->Internals->GridAxesActor->SetOrientation(orientation);
			this->Internals->GridAxesActor->SetVisibility(true);

			double center[4] = { 0, 0, 0, 1 };
			bbox.GetCenter(center);

			this->Internals->GridAxesActor->SetPosition(center);

			double a, b, c, x, y, z;
			bbox.GetBounds(a, b, c, x, y, z);
			Internals->GridAxesActor->SetGridBounds(a, b, c, x, y, z);

			Internals->GridAxesActor->SetXTitle("X Axis");
			Internals->GridAxesActor->SetYTitle("Y Axis");
			Internals->GridAxesActor->SetZTitle("Z Axis");

			//this->Internals->GridAxesConfigured = true;
		}
	}
	this->Internals->GridAxesActor->SetVisibility(show);
	return true;
}
