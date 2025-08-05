#pragma once

#include <memory>
#include <vector>
#include <filesystem>

#include <vtkSmartPointer.h>

class vtkRenderWindowInteractor;
class vtkImporter;
class vtkF3DMetaImporter;

namespace DS {
class Settings;
}

namespace fs = std::filesystem;

namespace f3d
{
namespace detail
{
class window_impl;
class animationManager;

class scene_impl
{
public:

	scene_impl(DS::Settings* psettings, window_impl* window);
	~scene_impl();

	void add(const fs::path& filePath);
	void add(const std::vector<fs::path>& filePaths);
	void clear();
	bool supports(const fs::path& filePath);
	void Load(const std::vector<vtkSmartPointer<vtkImporter>>& importers);

	DS::Settings* settings = nullptr;
	window_impl* Window = nullptr;

	animationManager* AnimationManager = nullptr;
	vtkNew<vtkF3DMetaImporter> MetaImporter;
};

}
}
