#pragma once

#include <memory>
#include <vector>
#include <filesystem>
#include <numeric>

#include <vtkSmartPointer.h>

class vtkRenderWindowInteractor;
class vtkImporter;
class vtkF3DMetaImporter;

namespace DS {
class Manager;
}

namespace fs = std::filesystem;

namespace f3d
{
namespace detail
{
class window_impl;
class scene_impl
{
public:

	scene_impl(DS::Manager* pmanager, window_impl* window);
	~scene_impl();

	void add(const fs::path& filePath);
	void add(const std::vector<fs::path>& filePaths);
	void clear();
	bool supports(const fs::path& filePath);
	void Load(const std::vector<vtkSmartPointer<vtkImporter>>& importers);

	DS::Manager* manager = nullptr;
	window_impl* Window = nullptr;

	vtkNew<vtkF3DMetaImporter> MetaImporter;
};

}
}
