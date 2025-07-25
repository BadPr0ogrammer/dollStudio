#ifndef f3d_options_generated_h
#define f3d_options_generated_h

#include "macros.h"
#include "options.h"
#include "options_tools.h"
#include "types.h"

// Some options could be marked as deprecated so we need to silent the warnings
F3D_SILENT_WARNING_PUSH()
F3D_SILENT_WARNING_DECL(4996, "deprecated-declarations")

namespace f3d
{
namespace options_generated
{
//----------------------------------------------------------------------------
/**
 * Generated method, see `options::set`
 */
void set(options& opt, std::string_view name, const option_variant_t& value)
{
  try
  {
    // clang-format off
    if (name == "interactor.invert_zoom") opt.interactor.invert_zoom = {std::get<bool>(value)};
    else if (name == "interactor.trackball") opt.interactor.trackball = {std::get<bool>(value)};
    else if (name == "model.color.opacity") opt.model.color.opacity = {std::get<double>(value)};
    else if (name == "model.color.rgb") opt.model.color.rgb = f3d::color_t{std::get<std::vector<double>>(value)};
    else if (name == "model.color.texture") opt.model.color.texture = {std::get<std::string>(value)};
    else if (name == "model.emissive.factor") opt.model.emissive.factor = f3d::color_t{std::get<std::vector<double>>(value)};
    else if (name == "model.emissive.texture") opt.model.emissive.texture = {std::get<std::string>(value)};
    else if (name == "model.matcap.texture") opt.model.matcap.texture = {std::get<std::string>(value)};
    else if (name == "model.material.base_ior") opt.model.material.base_ior = {std::get<double>(value)};
    else if (name == "model.material.metallic") opt.model.material.metallic = {std::get<double>(value)};
    else if (name == "model.material.roughness") opt.model.material.roughness = {std::get<double>(value)};
    else if (name == "model.material.texture") opt.model.material.texture = {std::get<std::string>(value)};
    else if (name == "model.normal.scale") opt.model.normal.scale = {std::get<double>(value)};
    else if (name == "model.normal.texture") opt.model.normal.texture = {std::get<std::string>(value)};
    else if (name == "model.point_sprites.enable") opt.model.point_sprites.enable = {std::get<bool>(value)};
    else if (name == "model.point_sprites.size") opt.model.point_sprites.size = {std::get<double>(value)};
    else if (name == "model.point_sprites.type") opt.model.point_sprites.type = {std::get<std::string>(value)};
    else if (name == "model.scivis.array_name") opt.model.scivis.array_name = {std::get<std::string>(value)};
    else if (name == "model.scivis.cells") opt.model.scivis.cells = {std::get<bool>(value)};
    else if (name == "model.scivis.colormap") opt.model.scivis.colormap = f3d::colormap_t{std::get<std::vector<double>>(value)};
    else if (name == "model.scivis.component") opt.model.scivis.component = {std::get<int>(value)};
    else if (name == "model.scivis.discretization") opt.model.scivis.discretization = {std::get<int>(value)};
    else if (name == "model.scivis.enable") opt.model.scivis.enable = {std::get<bool>(value)};
    else if (name == "model.scivis.range") opt.model.scivis.range = {std::get<std::vector<double>>(value)};
    else if (name == "model.textures_transform") opt.model.textures_transform = f3d::transform2d_t{std::get<std::vector<double>>(value)};
    else if (name == "model.volume.enable") opt.model.volume.enable = {std::get<bool>(value)};
    else if (name == "model.volume.inverse") opt.model.volume.inverse = {std::get<bool>(value)};
    else if (name == "render.armature.enable") opt.render.armature.enable = {std::get<bool>(value)};
    else if (name == "render.axes_grid.enable") opt.render.axes_grid.enable = {std::get<bool>(value)};
    else if (name == "render.backface_type") opt.render.backface_type = {std::get<std::string>(value)};
    else if (name == "render.background.blur.coc") opt.render.background.blur.coc = {std::get<double>(value)};
    else if (name == "render.background.blur.enable") opt.render.background.blur.enable = {std::get<bool>(value)};
    else if (name == "render.background.color") opt.render.background.color = f3d::color_t{std::get<std::vector<double>>(value)};
    else if (name == "render.background.skybox") opt.render.background.skybox = {std::get<bool>(value)};
    else if (name == "render.effect.ambient_occlusion") opt.render.effect.ambient_occlusion = {std::get<bool>(value)};
    else if (name == "render.effect.anti_aliasing") opt.render.effect.anti_aliasing = {std::get<bool>(value)};
    else if (name == "render.effect.antialiasing.enable") opt.render.effect.antialiasing.enable = {std::get<bool>(value)};
    else if (name == "render.effect.antialiasing.mode") opt.render.effect.antialiasing.mode = {std::get<std::string>(value)};
    else if (name == "render.effect.final_shader") opt.render.effect.final_shader = {std::get<std::string>(value)};
    else if (name == "render.effect.tone_mapping") opt.render.effect.tone_mapping = {std::get<bool>(value)};
    else if (name == "render.effect.translucency_support") opt.render.effect.translucency_support = {std::get<bool>(value)};
    else if (name == "render.grid.absolute") opt.render.grid.absolute = {std::get<bool>(value)};
    else if (name == "render.grid.color") opt.render.grid.color = f3d::color_t{std::get<std::vector<double>>(value)};
    else if (name == "render.grid.enable") opt.render.grid.enable = {std::get<bool>(value)};
    else if (name == "render.grid.subdivisions") opt.render.grid.subdivisions = {std::get<int>(value)};
    else if (name == "render.grid.unit") opt.render.grid.unit = {std::get<double>(value)};
    else if (name == "render.hdri.ambient") opt.render.hdri.ambient = {std::get<bool>(value)};
    else if (name == "render.hdri.file") opt.render.hdri.file = {std::get<std::string>(value)};
    else if (name == "render.light.intensity") opt.render.light.intensity = {std::get<double>(value)};
    else if (name == "render.line_width") opt.render.line_width = {std::get<double>(value)};
    else if (name == "render.point_size") opt.render.point_size = {std::get<double>(value)};
    else if (name == "render.raytracing.denoise") opt.render.raytracing.denoise = {std::get<bool>(value)};
    else if (name == "render.raytracing.enable") opt.render.raytracing.enable = {std::get<bool>(value)};
    else if (name == "render.raytracing.samples") opt.render.raytracing.samples = {std::get<int>(value)};
    else if (name == "render.show_edges") opt.render.show_edges = {std::get<bool>(value)};
    else if (name == "scene.animation.autoplay") opt.scene.animation.autoplay = {std::get<bool>(value)};
    else if (name == "scene.animation.index") opt.scene.animation.index = {std::get<int>(value)};
    else if (name == "scene.animation.indices") opt.scene.animation.indices = {std::get<std::vector<int>>(value)};
    else if (name == "scene.animation.speed_factor") opt.scene.animation.speed_factor = f3d::ratio_t{std::get<double>(value)};
    else if (name == "scene.camera.index") opt.scene.camera.index = {std::get<int>(value)};
    else if (name == "scene.camera.orthographic") opt.scene.camera.orthographic = {std::get<bool>(value)};
    else if (name == "scene.force_reader") opt.scene.force_reader = {std::get<std::string>(value)};
    else if (name == "scene.up_direction") opt.scene.up_direction = f3d::direction_t{std::get<std::vector<double>>(value)};
    else if (name == "ui.animation_progress") opt.ui.animation_progress = {std::get<bool>(value)};
    else if (name == "ui.axis") opt.ui.axis = {std::get<bool>(value)};
    else if (name == "ui.cheatsheet") opt.ui.cheatsheet = {std::get<bool>(value)};
    else if (name == "ui.console") opt.ui.console = {std::get<bool>(value)};
    else if (name == "ui.dropzone") opt.ui.dropzone = {std::get<bool>(value)};
    else if (name == "ui.dropzone_info") opt.ui.dropzone_info = {std::get<std::string>(value)};
    else if (name == "ui.filename") opt.ui.filename = {std::get<bool>(value)};
    else if (name == "ui.filename_info") opt.ui.filename_info = {std::get<std::string>(value)};
    else if (name == "ui.font_file") opt.ui.font_file = {std::get<std::string>(value)};
    else if (name == "ui.fps") opt.ui.fps = {std::get<bool>(value)};
    else if (name == "ui.loader_progress") opt.ui.loader_progress = {std::get<bool>(value)};
    else if (name == "ui.metadata") opt.ui.metadata = {std::get<bool>(value)};
    else if (name == "ui.minimal_console") opt.ui.minimal_console = {std::get<bool>(value)};
    else if (name == "ui.scalar_bar") opt.ui.scalar_bar = {std::get<bool>(value)};
    else if (name == "ui.scale") opt.ui.scale = f3d::ratio_t{std::get<double>(value)};
    // clang-format on
    else throw options::inexistent_exception("Option " + std::string(name) + " does not exist");
  }
  catch (const std::bad_variant_access&)
  {
    throw options::incompatible_exception(
      "Trying to set " + std::string(name) + " with incompatible type");
  }
  catch (const f3d::type_construction_exception& ex)
  {
    throw options::incompatible_exception(
      "Could not create type for " + std::string(name) + " : " + ex.what());
  }
}

//----------------------------------------------------------------------------
/**
 * Generated method, see `options::get`
 */
option_variant_t get(const options& opt, std::string_view name)
{
  try
  {
    // clang-format off
    if (name == "interactor.invert_zoom") return opt.interactor.invert_zoom;
    else if (name == "interactor.trackball") return opt.interactor.trackball;
    else if (name == "model.color.opacity") return opt.model.color.opacity.value();
    else if (name == "model.color.rgb") return opt.model.color.rgb.value();
    else if (name == "model.color.texture") return opt.model.color.texture.value().string();
    else if (name == "model.emissive.factor") return opt.model.emissive.factor.value();
    else if (name == "model.emissive.texture") return opt.model.emissive.texture.value().string();
    else if (name == "model.matcap.texture") return opt.model.matcap.texture.value().string();
    else if (name == "model.material.base_ior") return opt.model.material.base_ior.value();
    else if (name == "model.material.metallic") return opt.model.material.metallic.value();
    else if (name == "model.material.roughness") return opt.model.material.roughness.value();
    else if (name == "model.material.texture") return opt.model.material.texture.value().string();
    else if (name == "model.normal.scale") return opt.model.normal.scale.value();
    else if (name == "model.normal.texture") return opt.model.normal.texture.value().string();
    else if (name == "model.point_sprites.enable") return opt.model.point_sprites.enable;
    else if (name == "model.point_sprites.size") return opt.model.point_sprites.size;
    else if (name == "model.point_sprites.type") return opt.model.point_sprites.type;
    else if (name == "model.scivis.array_name") return opt.model.scivis.array_name.value();
    else if (name == "model.scivis.cells") return opt.model.scivis.cells;
    else if (name == "model.scivis.colormap") return opt.model.scivis.colormap;
    else if (name == "model.scivis.component") return opt.model.scivis.component;
    else if (name == "model.scivis.discretization") return opt.model.scivis.discretization.value();
    else if (name == "model.scivis.enable") return opt.model.scivis.enable;
    else if (name == "model.scivis.range") return opt.model.scivis.range.value();
    else if (name == "model.textures_transform") return opt.model.textures_transform.value();
    else if (name == "model.volume.enable") return opt.model.volume.enable;
    else if (name == "model.volume.inverse") return opt.model.volume.inverse;
    else if (name == "render.armature.enable") return opt.render.armature.enable;
    else if (name == "render.axes_grid.enable") return opt.render.axes_grid.enable;
    else if (name == "render.backface_type") return opt.render.backface_type.value();
    else if (name == "render.background.blur.coc") return opt.render.background.blur.coc;
    else if (name == "render.background.blur.enable") return opt.render.background.blur.enable;
    else if (name == "render.background.color") return opt.render.background.color;
    else if (name == "render.background.skybox") return opt.render.background.skybox;
    else if (name == "render.effect.ambient_occlusion") return opt.render.effect.ambient_occlusion;
    else if (name == "render.effect.anti_aliasing") return opt.render.effect.anti_aliasing;
    else if (name == "render.effect.antialiasing.enable") return opt.render.effect.antialiasing.enable;
    else if (name == "render.effect.antialiasing.mode") return opt.render.effect.antialiasing.mode;
    else if (name == "render.effect.final_shader") return opt.render.effect.final_shader.value();
    else if (name == "render.effect.tone_mapping") return opt.render.effect.tone_mapping;
    else if (name == "render.effect.translucency_support") return opt.render.effect.translucency_support;
    else if (name == "render.grid.absolute") return opt.render.grid.absolute;
    else if (name == "render.grid.color") return opt.render.grid.color;
    else if (name == "render.grid.enable") return opt.render.grid.enable;
    else if (name == "render.grid.subdivisions") return opt.render.grid.subdivisions;
    else if (name == "render.grid.unit") return opt.render.grid.unit.value();
    else if (name == "render.hdri.ambient") return opt.render.hdri.ambient;
    else if (name == "render.hdri.file") return opt.render.hdri.file.value().string();
    else if (name == "render.light.intensity") return opt.render.light.intensity;
    else if (name == "render.line_width") return opt.render.line_width.value();
    else if (name == "render.point_size") return opt.render.point_size.value();
    else if (name == "render.raytracing.denoise") return opt.render.raytracing.denoise;
    else if (name == "render.raytracing.enable") return opt.render.raytracing.enable;
    else if (name == "render.raytracing.samples") return opt.render.raytracing.samples;
    else if (name == "render.show_edges") return opt.render.show_edges.value();
    else if (name == "scene.animation.autoplay") return opt.scene.animation.autoplay;
    else if (name == "scene.animation.index") return opt.scene.animation.index;
    else if (name == "scene.animation.indices") return opt.scene.animation.indices;
    else if (name == "scene.animation.speed_factor") return opt.scene.animation.speed_factor;
    else if (name == "scene.camera.index") return opt.scene.camera.index.value();
    else if (name == "scene.camera.orthographic") return opt.scene.camera.orthographic.value();
    else if (name == "scene.force_reader") return opt.scene.force_reader.value();
    else if (name == "scene.up_direction") return opt.scene.up_direction;
    else if (name == "ui.animation_progress") return opt.ui.animation_progress;
    else if (name == "ui.axis") return opt.ui.axis;
    else if (name == "ui.cheatsheet") return opt.ui.cheatsheet;
    else if (name == "ui.console") return opt.ui.console;
    else if (name == "ui.dropzone") return opt.ui.dropzone;
    else if (name == "ui.dropzone_info") return opt.ui.dropzone_info;
    else if (name == "ui.filename") return opt.ui.filename;
    else if (name == "ui.filename_info") return opt.ui.filename_info;
    else if (name == "ui.font_file") return opt.ui.font_file.value().string();
    else if (name == "ui.fps") return opt.ui.fps;
    else if (name == "ui.loader_progress") return opt.ui.loader_progress;
    else if (name == "ui.metadata") return opt.ui.metadata;
    else if (name == "ui.minimal_console") return opt.ui.minimal_console;
    else if (name == "ui.scalar_bar") return opt.ui.scalar_bar;
    else if (name == "ui.scale") return opt.ui.scale;
    // clang-format on
    else throw options::inexistent_exception("Option " + std::string(name) + " does not exist");
  }
  catch (const std::bad_optional_access&)
  {
    throw options::no_value_exception("Trying to get " + std::string(name) + " before it was set");
  }
}

//----------------------------------------------------------------------------
/**
 * Generated method, see `options::getNames`
 */
std::vector<std::string> getNames()
{
  // clang-format off
  return { "interactor.invert_zoom",
  "interactor.trackball",
  "model.color.opacity",
  "model.color.rgb",
  "model.color.texture",
  "model.emissive.factor",
  "model.emissive.texture",
  "model.matcap.texture",
  "model.material.base_ior",
  "model.material.metallic",
  "model.material.roughness",
  "model.material.texture",
  "model.normal.scale",
  "model.normal.texture",
  "model.point_sprites.enable",
  "model.point_sprites.size",
  "model.point_sprites.type",
  "model.scivis.array_name",
  "model.scivis.cells",
  "model.scivis.colormap",
  "model.scivis.component",
  "model.scivis.discretization",
  "model.scivis.enable",
  "model.scivis.range",
  "model.textures_transform",
  "model.volume.enable",
  "model.volume.inverse",
  "render.armature.enable",
  "render.axes_grid.enable",
  "render.backface_type",
  "render.background.blur.coc",
  "render.background.blur.enable",
  "render.background.color",
  "render.background.skybox",
  "render.effect.ambient_occlusion",
  "render.effect.anti_aliasing",
  "render.effect.antialiasing.enable",
  "render.effect.antialiasing.mode",
  "render.effect.final_shader",
  "render.effect.tone_mapping",
  "render.effect.translucency_support",
  "render.grid.absolute",
  "render.grid.color",
  "render.grid.enable",
  "render.grid.subdivisions",
  "render.grid.unit",
  "render.hdri.ambient",
  "render.hdri.file",
  "render.light.intensity",
  "render.line_width",
  "render.point_size",
  "render.raytracing.denoise",
  "render.raytracing.enable",
  "render.raytracing.samples",
  "render.show_edges",
  "scene.animation.autoplay",
  "scene.animation.index",
  "scene.animation.indices",
  "scene.animation.speed_factor",
  "scene.camera.index",
  "scene.camera.orthographic",
  "scene.force_reader",
  "scene.up_direction",
  "ui.animation_progress",
  "ui.axis",
  "ui.cheatsheet",
  "ui.console",
  "ui.dropzone",
  "ui.dropzone_info",
  "ui.filename",
  "ui.filename_info",
  "ui.font_file",
  "ui.fps",
  "ui.loader_progress",
  "ui.metadata",
  "ui.minimal_console",
  "ui.scalar_bar",
  "ui.scale" };
  // clang-format on
}

//----------------------------------------------------------------------------
/**
 * Generated method, see `options::setAsString`
 */
void setAsString(options& opt, std::string_view name, const std::string& str)
{
  // clang-format off
  if (name == "interactor.invert_zoom") opt.interactor.invert_zoom = options_tools::parse<bool>(str);
  else if (name == "interactor.trackball") opt.interactor.trackball = options_tools::parse<bool>(str);
  else if (name == "model.color.opacity") opt.model.color.opacity = options_tools::parse<double>(str);
  else if (name == "model.color.rgb") opt.model.color.rgb = options_tools::parse<f3d::color_t>(str);
  else if (name == "model.color.texture") opt.model.color.texture = options_tools::parse<std::filesystem::path>(str);
  else if (name == "model.emissive.factor") opt.model.emissive.factor = options_tools::parse<f3d::color_t>(str);
  else if (name == "model.emissive.texture") opt.model.emissive.texture = options_tools::parse<std::filesystem::path>(str);
  else if (name == "model.matcap.texture") opt.model.matcap.texture = options_tools::parse<std::filesystem::path>(str);
  else if (name == "model.material.base_ior") opt.model.material.base_ior = options_tools::parse<double>(str);
  else if (name == "model.material.metallic") opt.model.material.metallic = options_tools::parse<double>(str);
  else if (name == "model.material.roughness") opt.model.material.roughness = options_tools::parse<double>(str);
  else if (name == "model.material.texture") opt.model.material.texture = options_tools::parse<std::filesystem::path>(str);
  else if (name == "model.normal.scale") opt.model.normal.scale = options_tools::parse<double>(str);
  else if (name == "model.normal.texture") opt.model.normal.texture = options_tools::parse<std::filesystem::path>(str);
  else if (name == "model.point_sprites.enable") opt.model.point_sprites.enable = options_tools::parse<bool>(str);
  else if (name == "model.point_sprites.size") opt.model.point_sprites.size = options_tools::parse<double>(str);
  else if (name == "model.point_sprites.type") opt.model.point_sprites.type = options_tools::parse<std::string>(str);
  else if (name == "model.scivis.array_name") opt.model.scivis.array_name = options_tools::parse<std::string>(str);
  else if (name == "model.scivis.cells") opt.model.scivis.cells = options_tools::parse<bool>(str);
  else if (name == "model.scivis.colormap") opt.model.scivis.colormap = options_tools::parse<f3d::colormap_t>(str);
  else if (name == "model.scivis.component") opt.model.scivis.component = options_tools::parse<int>(str);
  else if (name == "model.scivis.discretization") opt.model.scivis.discretization = options_tools::parse<int>(str);
  else if (name == "model.scivis.enable") opt.model.scivis.enable = options_tools::parse<bool>(str);
  else if (name == "model.scivis.range") opt.model.scivis.range = options_tools::parse<std::vector<double>>(str);
  else if (name == "model.textures_transform") opt.model.textures_transform = options_tools::parse<f3d::transform2d_t>(str);
  else if (name == "model.volume.enable") opt.model.volume.enable = options_tools::parse<bool>(str);
  else if (name == "model.volume.inverse") opt.model.volume.inverse = options_tools::parse<bool>(str);
  else if (name == "render.armature.enable") opt.render.armature.enable = options_tools::parse<bool>(str);
  else if (name == "render.axes_grid.enable") opt.render.axes_grid.enable = options_tools::parse<bool>(str);
  else if (name == "render.backface_type") opt.render.backface_type = options_tools::parse<std::string>(str);
  else if (name == "render.background.blur.coc") opt.render.background.blur.coc = options_tools::parse<double>(str);
  else if (name == "render.background.blur.enable") opt.render.background.blur.enable = options_tools::parse<bool>(str);
  else if (name == "render.background.color") opt.render.background.color = options_tools::parse<f3d::color_t>(str);
  else if (name == "render.background.skybox") opt.render.background.skybox = options_tools::parse<bool>(str);
  else if (name == "render.effect.ambient_occlusion") opt.render.effect.ambient_occlusion = options_tools::parse<bool>(str);
  else if (name == "render.effect.anti_aliasing") opt.render.effect.anti_aliasing = options_tools::parse<bool>(str);
  else if (name == "render.effect.antialiasing.enable") opt.render.effect.antialiasing.enable = options_tools::parse<bool>(str);
  else if (name == "render.effect.antialiasing.mode") opt.render.effect.antialiasing.mode = options_tools::parse<std::string>(str);
  else if (name == "render.effect.final_shader") opt.render.effect.final_shader = options_tools::parse<std::string>(str);
  else if (name == "render.effect.tone_mapping") opt.render.effect.tone_mapping = options_tools::parse<bool>(str);
  else if (name == "render.effect.translucency_support") opt.render.effect.translucency_support = options_tools::parse<bool>(str);
  else if (name == "render.grid.absolute") opt.render.grid.absolute = options_tools::parse<bool>(str);
  else if (name == "render.grid.color") opt.render.grid.color = options_tools::parse<f3d::color_t>(str);
  else if (name == "render.grid.enable") opt.render.grid.enable = options_tools::parse<bool>(str);
  else if (name == "render.grid.subdivisions") opt.render.grid.subdivisions = options_tools::parse<int>(str);
  else if (name == "render.grid.unit") opt.render.grid.unit = options_tools::parse<double>(str);
  else if (name == "render.hdri.ambient") opt.render.hdri.ambient = options_tools::parse<bool>(str);
  else if (name == "render.hdri.file") opt.render.hdri.file = options_tools::parse<std::filesystem::path>(str);
  else if (name == "render.light.intensity") opt.render.light.intensity = options_tools::parse<double>(str);
  else if (name == "render.line_width") opt.render.line_width = options_tools::parse<double>(str);
  else if (name == "render.point_size") opt.render.point_size = options_tools::parse<double>(str);
  else if (name == "render.raytracing.denoise") opt.render.raytracing.denoise = options_tools::parse<bool>(str);
  else if (name == "render.raytracing.enable") opt.render.raytracing.enable = options_tools::parse<bool>(str);
  else if (name == "render.raytracing.samples") opt.render.raytracing.samples = options_tools::parse<int>(str);
  else if (name == "render.show_edges") opt.render.show_edges = options_tools::parse<bool>(str);
  else if (name == "scene.animation.autoplay") opt.scene.animation.autoplay = options_tools::parse<bool>(str);
  else if (name == "scene.animation.index") opt.scene.animation.index = options_tools::parse<int>(str);
  else if (name == "scene.animation.indices") opt.scene.animation.indices = options_tools::parse<std::vector<int>>(str);
  else if (name == "scene.animation.speed_factor") opt.scene.animation.speed_factor = options_tools::parse<f3d::ratio_t>(str);
  else if (name == "scene.camera.index") opt.scene.camera.index = options_tools::parse<int>(str);
  else if (name == "scene.camera.orthographic") opt.scene.camera.orthographic = options_tools::parse<bool>(str);
  else if (name == "scene.force_reader") opt.scene.force_reader = options_tools::parse<std::string>(str);
  else if (name == "scene.up_direction") opt.scene.up_direction = options_tools::parse<f3d::direction_t>(str);
  else if (name == "ui.animation_progress") opt.ui.animation_progress = options_tools::parse<bool>(str);
  else if (name == "ui.axis") opt.ui.axis = options_tools::parse<bool>(str);
  else if (name == "ui.cheatsheet") opt.ui.cheatsheet = options_tools::parse<bool>(str);
  else if (name == "ui.console") opt.ui.console = options_tools::parse<bool>(str);
  else if (name == "ui.dropzone") opt.ui.dropzone = options_tools::parse<bool>(str);
  else if (name == "ui.dropzone_info") opt.ui.dropzone_info = options_tools::parse<std::string>(str);
  else if (name == "ui.filename") opt.ui.filename = options_tools::parse<bool>(str);
  else if (name == "ui.filename_info") opt.ui.filename_info = options_tools::parse<std::string>(str);
  else if (name == "ui.font_file") opt.ui.font_file = options_tools::parse<std::filesystem::path>(str);
  else if (name == "ui.fps") opt.ui.fps = options_tools::parse<bool>(str);
  else if (name == "ui.loader_progress") opt.ui.loader_progress = options_tools::parse<bool>(str);
  else if (name == "ui.metadata") opt.ui.metadata = options_tools::parse<bool>(str);
  else if (name == "ui.minimal_console") opt.ui.minimal_console = options_tools::parse<bool>(str);
  else if (name == "ui.scalar_bar") opt.ui.scalar_bar = options_tools::parse<bool>(str);
  else if (name == "ui.scale") opt.ui.scale = options_tools::parse<f3d::ratio_t>(str);
  // clang-format on
  else throw options::inexistent_exception("Option " + std::string(name) + " does not exist");
}
//----------------------------------------------------------------------------
/**
 * Generated method, see `options::getAsString`
 */
std::string getAsString(const options& opt, std::string_view name)
{
  try
  {
    // clang-format off
    if (name == "interactor.invert_zoom") return options_tools::format(opt.interactor.invert_zoom);
    else if (name == "interactor.trackball") return options_tools::format(opt.interactor.trackball);
    else if (name == "model.color.opacity") return options_tools::format(opt.model.color.opacity.value());
    else if (name == "model.color.rgb") return options_tools::format(opt.model.color.rgb.value());
    else if (name == "model.color.texture") return options_tools::format(opt.model.color.texture.value());
    else if (name == "model.emissive.factor") return options_tools::format(opt.model.emissive.factor.value());
    else if (name == "model.emissive.texture") return options_tools::format(opt.model.emissive.texture.value());
    else if (name == "model.matcap.texture") return options_tools::format(opt.model.matcap.texture.value());
    else if (name == "model.material.base_ior") return options_tools::format(opt.model.material.base_ior.value());
    else if (name == "model.material.metallic") return options_tools::format(opt.model.material.metallic.value());
    else if (name == "model.material.roughness") return options_tools::format(opt.model.material.roughness.value());
    else if (name == "model.material.texture") return options_tools::format(opt.model.material.texture.value());
    else if (name == "model.normal.scale") return options_tools::format(opt.model.normal.scale.value());
    else if (name == "model.normal.texture") return options_tools::format(opt.model.normal.texture.value());
    else if (name == "model.point_sprites.enable") return options_tools::format(opt.model.point_sprites.enable);
    else if (name == "model.point_sprites.size") return options_tools::format(opt.model.point_sprites.size);
    else if (name == "model.point_sprites.type") return options_tools::format(opt.model.point_sprites.type);
    else if (name == "model.scivis.array_name") return options_tools::format(opt.model.scivis.array_name.value());
    else if (name == "model.scivis.cells") return options_tools::format(opt.model.scivis.cells);
    else if (name == "model.scivis.colormap") return options_tools::format(opt.model.scivis.colormap);
    else if (name == "model.scivis.component") return options_tools::format(opt.model.scivis.component);
    else if (name == "model.scivis.discretization") return options_tools::format(opt.model.scivis.discretization.value());
    else if (name == "model.scivis.enable") return options_tools::format(opt.model.scivis.enable);
    else if (name == "model.scivis.range") return options_tools::format(opt.model.scivis.range.value());
    else if (name == "model.textures_transform") return options_tools::format(opt.model.textures_transform.value());
    else if (name == "model.volume.enable") return options_tools::format(opt.model.volume.enable);
    else if (name == "model.volume.inverse") return options_tools::format(opt.model.volume.inverse);
    else if (name == "render.armature.enable") return options_tools::format(opt.render.armature.enable);
    else if (name == "render.axes_grid.enable") return options_tools::format(opt.render.axes_grid.enable);
    else if (name == "render.backface_type") return options_tools::format(opt.render.backface_type.value());
    else if (name == "render.background.blur.coc") return options_tools::format(opt.render.background.blur.coc);
    else if (name == "render.background.blur.enable") return options_tools::format(opt.render.background.blur.enable);
    else if (name == "render.background.color") return options_tools::format(opt.render.background.color);
    else if (name == "render.background.skybox") return options_tools::format(opt.render.background.skybox);
    else if (name == "render.effect.ambient_occlusion") return options_tools::format(opt.render.effect.ambient_occlusion);
    else if (name == "render.effect.anti_aliasing") return options_tools::format(opt.render.effect.anti_aliasing);
    else if (name == "render.effect.antialiasing.enable") return options_tools::format(opt.render.effect.antialiasing.enable);
    else if (name == "render.effect.antialiasing.mode") return options_tools::format(opt.render.effect.antialiasing.mode);
    else if (name == "render.effect.final_shader") return options_tools::format(opt.render.effect.final_shader.value());
    else if (name == "render.effect.tone_mapping") return options_tools::format(opt.render.effect.tone_mapping);
    else if (name == "render.effect.translucency_support") return options_tools::format(opt.render.effect.translucency_support);
    else if (name == "render.grid.absolute") return options_tools::format(opt.render.grid.absolute);
    else if (name == "render.grid.color") return options_tools::format(opt.render.grid.color);
    else if (name == "render.grid.enable") return options_tools::format(opt.render.grid.enable);
    else if (name == "render.grid.subdivisions") return options_tools::format(opt.render.grid.subdivisions);
    else if (name == "render.grid.unit") return options_tools::format(opt.render.grid.unit.value());
    else if (name == "render.hdri.ambient") return options_tools::format(opt.render.hdri.ambient);
    else if (name == "render.hdri.file") return options_tools::format(opt.render.hdri.file.value());
    else if (name == "render.light.intensity") return options_tools::format(opt.render.light.intensity);
    else if (name == "render.line_width") return options_tools::format(opt.render.line_width.value());
    else if (name == "render.point_size") return options_tools::format(opt.render.point_size.value());
    else if (name == "render.raytracing.denoise") return options_tools::format(opt.render.raytracing.denoise);
    else if (name == "render.raytracing.enable") return options_tools::format(opt.render.raytracing.enable);
    else if (name == "render.raytracing.samples") return options_tools::format(opt.render.raytracing.samples);
    else if (name == "render.show_edges") return options_tools::format(opt.render.show_edges.value());
    else if (name == "scene.animation.autoplay") return options_tools::format(opt.scene.animation.autoplay);
    else if (name == "scene.animation.index") return options_tools::format(opt.scene.animation.index);
    else if (name == "scene.animation.indices") return options_tools::format(opt.scene.animation.indices);
    else if (name == "scene.animation.speed_factor") return options_tools::format(opt.scene.animation.speed_factor);
    else if (name == "scene.camera.index") return options_tools::format(opt.scene.camera.index.value());
    else if (name == "scene.camera.orthographic") return options_tools::format(opt.scene.camera.orthographic.value());
    else if (name == "scene.force_reader") return options_tools::format(opt.scene.force_reader.value());
    else if (name == "scene.up_direction") return options_tools::format(opt.scene.up_direction);
    else if (name == "ui.animation_progress") return options_tools::format(opt.ui.animation_progress);
    else if (name == "ui.axis") return options_tools::format(opt.ui.axis);
    else if (name == "ui.cheatsheet") return options_tools::format(opt.ui.cheatsheet);
    else if (name == "ui.console") return options_tools::format(opt.ui.console);
    else if (name == "ui.dropzone") return options_tools::format(opt.ui.dropzone);
    else if (name == "ui.dropzone_info") return options_tools::format(opt.ui.dropzone_info);
    else if (name == "ui.filename") return options_tools::format(opt.ui.filename);
    else if (name == "ui.filename_info") return options_tools::format(opt.ui.filename_info);
    else if (name == "ui.font_file") return options_tools::format(opt.ui.font_file.value());
    else if (name == "ui.fps") return options_tools::format(opt.ui.fps);
    else if (name == "ui.loader_progress") return options_tools::format(opt.ui.loader_progress);
    else if (name == "ui.metadata") return options_tools::format(opt.ui.metadata);
    else if (name == "ui.minimal_console") return options_tools::format(opt.ui.minimal_console);
    else if (name == "ui.scalar_bar") return options_tools::format(opt.ui.scalar_bar);
    else if (name == "ui.scale") return options_tools::format(opt.ui.scale);
    // clang-format on
    else throw options::inexistent_exception("Option " + std::string(name) + " does not exist");
  }
  catch (const std::bad_optional_access&)
  {
    throw options::no_value_exception("Trying to get " + std::string(name) + " before it was set");
  }
}

//----------------------------------------------------------------------------
/**
 * Generated method, see `options::isOptional`
 */
bool isOptional(std::string_view name)
{
  // clang-format off
  if (name == "interactor.invert_zoom") return false;
  else if (name == "interactor.trackball") return false;
  else if (name == "model.color.opacity") return true;
  else if (name == "model.color.rgb") return true;
  else if (name == "model.color.texture") return true;
  else if (name == "model.emissive.factor") return true;
  else if (name == "model.emissive.texture") return true;
  else if (name == "model.matcap.texture") return true;
  else if (name == "model.material.base_ior") return true;
  else if (name == "model.material.metallic") return true;
  else if (name == "model.material.roughness") return true;
  else if (name == "model.material.texture") return true;
  else if (name == "model.normal.scale") return true;
  else if (name == "model.normal.texture") return true;
  else if (name == "model.point_sprites.enable") return false;
  else if (name == "model.point_sprites.size") return false;
  else if (name == "model.point_sprites.type") return false;
  else if (name == "model.scivis.array_name") return true;
  else if (name == "model.scivis.cells") return false;
  else if (name == "model.scivis.colormap") return false;
  else if (name == "model.scivis.component") return false;
  else if (name == "model.scivis.discretization") return true;
  else if (name == "model.scivis.enable") return false;
  else if (name == "model.scivis.range") return true;
  else if (name == "model.textures_transform") return true;
  else if (name == "model.volume.enable") return false;
  else if (name == "model.volume.inverse") return false;
  else if (name == "render.armature.enable") return false;
  else if (name == "render.axes_grid.enable") return false;
  else if (name == "render.backface_type") return true;
  else if (name == "render.background.blur.coc") return false;
  else if (name == "render.background.blur.enable") return false;
  else if (name == "render.background.color") return false;
  else if (name == "render.background.skybox") return false;
  else if (name == "render.effect.ambient_occlusion") return false;
  else if (name == "render.effect.anti_aliasing") return false;
  else if (name == "render.effect.antialiasing.enable") return false;
  else if (name == "render.effect.antialiasing.mode") return false;
  else if (name == "render.effect.final_shader") return true;
  else if (name == "render.effect.tone_mapping") return false;
  else if (name == "render.effect.translucency_support") return false;
  else if (name == "render.grid.absolute") return false;
  else if (name == "render.grid.color") return false;
  else if (name == "render.grid.enable") return false;
  else if (name == "render.grid.subdivisions") return false;
  else if (name == "render.grid.unit") return true;
  else if (name == "render.hdri.ambient") return false;
  else if (name == "render.hdri.file") return true;
  else if (name == "render.light.intensity") return false;
  else if (name == "render.line_width") return true;
  else if (name == "render.point_size") return true;
  else if (name == "render.raytracing.denoise") return false;
  else if (name == "render.raytracing.enable") return false;
  else if (name == "render.raytracing.samples") return false;
  else if (name == "render.show_edges") return true;
  else if (name == "scene.animation.autoplay") return false;
  else if (name == "scene.animation.index") return false;
  else if (name == "scene.animation.indices") return false;
  else if (name == "scene.animation.speed_factor") return false;
  else if (name == "scene.camera.index") return true;
  else if (name == "scene.camera.orthographic") return true;
  else if (name == "scene.force_reader") return true;
  else if (name == "scene.up_direction") return false;
  else if (name == "ui.animation_progress") return false;
  else if (name == "ui.axis") return false;
  else if (name == "ui.cheatsheet") return false;
  else if (name == "ui.console") return false;
  else if (name == "ui.dropzone") return false;
  else if (name == "ui.dropzone_info") return false;
  else if (name == "ui.filename") return false;
  else if (name == "ui.filename_info") return false;
  else if (name == "ui.font_file") return true;
  else if (name == "ui.fps") return false;
  else if (name == "ui.loader_progress") return false;
  else if (name == "ui.metadata") return false;
  else if (name == "ui.minimal_console") return false;
  else if (name == "ui.scalar_bar") return false;
  else if (name == "ui.scale") return false;
  // clang-format on
  else throw options::inexistent_exception("Option " + std::string(name) + " does not exist");
}

//----------------------------------------------------------------------------
/**
 * Generated method, see `options::reset`
 */
void reset(options& opt, std::string_view name)
{
  // clang-format off
  if (name == "interactor.invert_zoom") opt.interactor.invert_zoom = false;
  else if (name == "interactor.trackball") opt.interactor.trackball = false;
  else if (name == "model.color.opacity") opt.model.color.opacity.reset();
  else if (name == "model.color.rgb") opt.model.color.rgb.reset();
  else if (name == "model.color.texture") opt.model.color.texture.reset();
  else if (name == "model.emissive.factor") opt.model.emissive.factor.reset();
  else if (name == "model.emissive.texture") opt.model.emissive.texture.reset();
  else if (name == "model.matcap.texture") opt.model.matcap.texture.reset();
  else if (name == "model.material.base_ior") opt.model.material.base_ior.reset();
  else if (name == "model.material.metallic") opt.model.material.metallic.reset();
  else if (name == "model.material.roughness") opt.model.material.roughness.reset();
  else if (name == "model.material.texture") opt.model.material.texture.reset();
  else if (name == "model.normal.scale") opt.model.normal.scale.reset();
  else if (name == "model.normal.texture") opt.model.normal.texture.reset();
  else if (name == "model.point_sprites.enable") opt.model.point_sprites.enable = false;
  else if (name == "model.point_sprites.size") opt.model.point_sprites.size = 10.0;
  else if (name == "model.point_sprites.type") opt.model.point_sprites.type = "sphere";
  else if (name == "model.scivis.array_name") opt.model.scivis.array_name.reset();
  else if (name == "model.scivis.cells") opt.model.scivis.cells = false;
  else if (name == "model.scivis.colormap") opt.model.scivis.colormap = f3d::colormap_t{0.0, 0.0, 0.0, 0.0, 0.4, 0.9, 0.0, 0.0, 0.8, 0.9, 0.9, 0.0, 1.0, 1.0, 1.0, 1.0};
  else if (name == "model.scivis.component") opt.model.scivis.component = -1;
  else if (name == "model.scivis.discretization") opt.model.scivis.discretization.reset();
  else if (name == "model.scivis.enable") opt.model.scivis.enable = false;
  else if (name == "model.scivis.range") opt.model.scivis.range.reset();
  else if (name == "model.textures_transform") opt.model.textures_transform.reset();
  else if (name == "model.volume.enable") opt.model.volume.enable = false;
  else if (name == "model.volume.inverse") opt.model.volume.inverse = false;
  else if (name == "render.armature.enable") opt.render.armature.enable = false;
  else if (name == "render.axes_grid.enable") opt.render.axes_grid.enable = false;
  else if (name == "render.backface_type") opt.render.backface_type.reset();
  else if (name == "render.background.blur.coc") opt.render.background.blur.coc = 20.0;
  else if (name == "render.background.blur.enable") opt.render.background.blur.enable = false;
  else if (name == "render.background.color") opt.render.background.color = f3d::color_t{0.2, 0.2, 0.2};
  else if (name == "render.background.skybox") opt.render.background.skybox = false;
  else if (name == "render.effect.ambient_occlusion") opt.render.effect.ambient_occlusion = false;
  else if (name == "render.effect.anti_aliasing") opt.render.effect.anti_aliasing = false;
  else if (name == "render.effect.antialiasing.enable") opt.render.effect.antialiasing.enable = false;
  else if (name == "render.effect.antialiasing.mode") opt.render.effect.antialiasing.mode = "fxaa";
  else if (name == "render.effect.final_shader") opt.render.effect.final_shader.reset();
  else if (name == "render.effect.tone_mapping") opt.render.effect.tone_mapping = false;
  else if (name == "render.effect.translucency_support") opt.render.effect.translucency_support = false;
  else if (name == "render.grid.absolute") opt.render.grid.absolute = false;
  else if (name == "render.grid.color") opt.render.grid.color = f3d::color_t{0.0, 0.0, 0.0};
  else if (name == "render.grid.enable") opt.render.grid.enable = false;
  else if (name == "render.grid.subdivisions") opt.render.grid.subdivisions = 10;
  else if (name == "render.grid.unit") opt.render.grid.unit.reset();
  else if (name == "render.hdri.ambient") opt.render.hdri.ambient = false;
  else if (name == "render.hdri.file") opt.render.hdri.file.reset();
  else if (name == "render.light.intensity") opt.render.light.intensity = 1.0;
  else if (name == "render.line_width") opt.render.line_width.reset();
  else if (name == "render.point_size") opt.render.point_size.reset();
  else if (name == "render.raytracing.denoise") opt.render.raytracing.denoise = false;
  else if (name == "render.raytracing.enable") opt.render.raytracing.enable = false;
  else if (name == "render.raytracing.samples") opt.render.raytracing.samples = 5;
  else if (name == "render.show_edges") opt.render.show_edges.reset();
  else if (name == "scene.animation.autoplay") opt.scene.animation.autoplay = false;
  else if (name == "scene.animation.index") opt.scene.animation.index = 0;
  else if (name == "scene.animation.indices") opt.scene.animation.indices = {0};
  else if (name == "scene.animation.speed_factor") opt.scene.animation.speed_factor = f3d::ratio_t{1.0};
  else if (name == "scene.camera.index") opt.scene.camera.index.reset();
  else if (name == "scene.camera.orthographic") opt.scene.camera.orthographic.reset();
  else if (name == "scene.force_reader") opt.scene.force_reader.reset();
  else if (name == "scene.up_direction") opt.scene.up_direction = f3d::direction_t{0,1,0};
  else if (name == "ui.animation_progress") opt.ui.animation_progress = false;
  else if (name == "ui.axis") opt.ui.axis = false;
  else if (name == "ui.cheatsheet") opt.ui.cheatsheet = false;
  else if (name == "ui.console") opt.ui.console = false;
  else if (name == "ui.dropzone") opt.ui.dropzone = false;
  else if (name == "ui.dropzone_info") opt.ui.dropzone_info = "";
  else if (name == "ui.filename") opt.ui.filename = false;
  else if (name == "ui.filename_info") opt.ui.filename_info = "";
  else if (name == "ui.font_file") opt.ui.font_file.reset();
  else if (name == "ui.fps") opt.ui.fps = false;
  else if (name == "ui.loader_progress") opt.ui.loader_progress = false;
  else if (name == "ui.metadata") opt.ui.metadata = false;
  else if (name == "ui.minimal_console") opt.ui.minimal_console = false;
  else if (name == "ui.scalar_bar") opt.ui.scalar_bar = false;
  else if (name == "ui.scale") opt.ui.scale = f3d::ratio_t{1.0};
  // clang-format on
  else throw options::inexistent_exception("Option " + std::string(name) + " does not exist");
}

} // options_generated
} // f3d

F3D_SILENT_WARNING_POP()

#endif // f3d_options_generated_h
