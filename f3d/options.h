#ifndef f3d_options_h
#define f3d_options_h

#include "exception.h"
#include "export.h"
#include "types.h"

#include <array>
#include <filesystem>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace f3d
{
// Declared here for simplicity
using option_variant_t =
  std::variant<bool, int, double, std::string, std::vector<double>, std::vector<int>>;

/**
 * @class   options
 * @brief   Class used to control the different options
 *
 * A class to control the different options of f3d.
 * See the README_libf3d.md for the full listing of options.
 * The struct below can also be consulted.
 */
class /*F3D_EXPORT*/ options
{
public:
  ///@{ @name Constructors
  /**
   * Default/Copy/move constructors/operators.
   */
  options();
  ~options();
  options(const options& opt);
  options& operator=(const options& opt);
  options(options&& other) noexcept;
  options& operator=(options&& other) noexcept;
  ///@}

  /**
   * Set an option as a variant based on its name
   * Please note that, on Windows, using an explicit std::string may be required:
   * https://stackoverflow.com/questions/60681710/prefer-stdstring-in-stdvariantbool-stdstring-for-const-char
   * Throw an options::inexistent_exception if option does not exist.
   * Throw an options::incompatible_exception if value is not compatible with option.
   */
  options& set(std::string_view name, const option_variant_t& value);

  /**
   * Get an option as a variant based on its name
   * Throw an options::inexistent_exception if option does not exist.
   * Throw an options::no_value_exception if option has not been set.
   */
  [[nodiscard]] option_variant_t get(std::string_view name) const;

  /**
   * Set an option as a string based on its name
   * Use dedicated parsing code, see the related doc in PARSING.md.
   * Throw an options::inexistent_exception if option does not exist.
   * Throw an options::parsing_exception if parsing failed.
   */
  options& setAsString(std::string_view name, const std::string& str);

  /**
   * Get an option as a string based on its name
   * Throw an options::inexistent_exception if option does not exist.
   * Throw an options::no_value_exception if option has not been set.
   */
  [[nodiscard]] std::string getAsString(std::string_view name) const;

  /**
   * A boolean option specific method to toggle it.
   * If the option has not been set yet, set it to true.
   * Throw an options::inexistent_exception if option does not exist.
   * Throw an options::incompatible_exception if option is not boolean.
   */
  options& toggle(std::string_view name);

  /**
   * Compare an option between this and a provided other.
   * Return true if they are the same value, false otherwise.
   * Throw an options::inexistent_exception if option does not exist.
   */
  [[nodiscard]] bool isSame(const options& other, std::string_view name) const;

  /**
   * Return true if an option has a value, false otherwise
   * Always returns true for non-optional options.
   * Throw an options::inexistent_exception if option does not exist.
   */
  [[nodiscard]] bool hasValue(std::string_view name) const;

  /**
   * Copy the value of an option from this to the provided other.
   * Throw an options::inexistent_exception if option does not exist.
   */
  options& copy(const options& other, std::string_view name);

  /**
   * Get all available option names.
   */
  [[nodiscard]] static std::vector<std::string> getAllNames();

  /**
   * Get all option names that currently have values.
   */
  [[nodiscard]] std::vector<std::string> getNames() const;

  /**
   * Get the closest option name and its Levenshtein distance.
   */
  [[nodiscard]] std::pair<std::string, unsigned int> getClosestOption(
    std::string_view option) const;

  /**
   *  Returns true if the option is optional else returns false.
   *  Throws an options::inexistent_exception if option does not exist.
   */
  [[nodiscard]] bool isOptional(std::string_view option) const;

  /**
   *  Resets the option to default value.
   *  Throws an options::inexistent_exception if option does not exist.
   */
  options& reset(std::string_view name);

  /**
   *  Unset the option if it is optional else throws options::incompatible_exception.
   *  Throws an options::inexistent_exception if option does not exist.
   */
  options& removeValue(std::string_view name);

  /**
   * Templated parsing method used internally to parse strings.
   * Implemented for the different supported types,
   * see PARSING.md for more info.
   * Throw an options::parsing_exception if parsing failed.
   */
  template<typename T>
  [[nodiscard]] static T parse(const std::string& str);

  /**
   * Templated parsing method used internally to format var into strings.
   * Implemented for the different supported types,
   * see PARSING.md for more info.
   */
  template<typename T>
  [[nodiscard]] static std::string format(const T& var);

  /**
   * An exception that can be thrown by the options
   * when parsing of a string into an option value fails
   */
  struct parsing_exception : public exception
  {
    explicit parsing_exception(const std::string& what = "");
  };

  /**
   * An exception that can be thrown by the options
   * when an operation on a specific option is incompatible with
   * its internal type.
   */
  struct incompatible_exception : public exception
  {
    explicit incompatible_exception(const std::string& what = "");
  };

  /**
   * An exception that can be thrown by the options
   * when a provided option does not exist.
   */
  struct inexistent_exception : public exception
  {
    explicit inexistent_exception(const std::string& what = "");
  };

  /**
   * An exception that can be thrown by the options
   * when a provided option is accessed before being set.
   */
  struct no_value_exception : public exception
  {
    explicit no_value_exception(const std::string& what = "");
  };

  /**
   * The complete generated options struct
   */
  // clang-format off
    struct interactor {
    bool invert_zoom = false;
    bool trackball = false;
  } interactor;

  struct model {
    struct color {
      std::optional<double> opacity;
      std::optional<f3d::color_t> rgb;
      std::optional<std::filesystem::path> texture;
    } color;

    struct emissive {
      std::optional<f3d::color_t> factor;
      std::optional<std::filesystem::path> texture;
    } emissive;

    struct matcap {
      std::optional<std::filesystem::path> texture;
    } matcap;

    struct material {
      std::optional<double> base_ior;
      std::optional<double> metallic;
      std::optional<double> roughness;
      std::optional<std::filesystem::path> texture;
    } material;

    struct normal {
      std::optional<double> scale;
      std::optional<std::filesystem::path> texture;
    } normal;

    struct point_sprites {
      bool enable = false;
      double size = 10.0;
      std::string type = "sphere";
    } point_sprites;

    struct scivis {
      std::optional<std::string> array_name;
      bool cells = false;
      f3d::colormap_t colormap = f3d::colormap_t{0.0, 0.0, 0.0, 0.0, 0.4, 0.9, 0.0, 0.0, 0.8, 0.9, 0.9, 0.0, 1.0, 1.0, 1.0, 1.0};
      int component = -1;
      std::optional<int> discretization;
      bool enable = false;
      std::optional<std::vector<double>> range;
    } scivis;

    std::optional<f3d::transform2d_t> textures_transform;
    struct volume {
      bool enable = false;
      bool inverse = false;
    } volume;

  } model;

  struct render {
    struct armature {
      bool enable = false;
    } armature;

    struct axes_grid {
      bool enable = false;
    } axes_grid;

    std::optional<std::string> backface_type;
    struct background {
      struct blur {
        double coc = 20.0;
        bool enable = false;
      } blur;

      f3d::color_t color = f3d::color_t{0.2, 0.2, 0.2};
      bool skybox = false;
    } background;

    struct effect {
      bool ambient_occlusion = false;
      [[deprecated("use render.effect.antialiasing.enable instead")]] bool anti_aliasing = false;
      struct antialiasing {
        bool enable = false;
        std::string mode = "fxaa";
      } antialiasing;

      std::optional<std::string> final_shader;
      bool tone_mapping = false;
      bool translucency_support = false;
    } effect;

    struct grid {
      bool absolute = false;
      f3d::color_t color = f3d::color_t{0.0, 0.0, 0.0};
      bool enable = false;
      int subdivisions = 10;
      std::optional<double> unit;
    } grid;

    struct hdri {
      bool ambient = false;
      std::optional<std::filesystem::path> file;
    } hdri;

    struct light {
      double intensity = 1.0;
    } light;

    std::optional<double> line_width;
    std::optional<double> point_size;
    struct raytracing {
      bool denoise = false;
      bool enable = false;
      int samples = 5;
    } raytracing;

    std::optional<bool> show_edges;
  } render;

  struct scene {
    struct animation {
      bool autoplay = false;
      [[deprecated("use scene.animation.indices instead")]] int index = 0;
      std::vector<int> indices = {0};
      f3d::ratio_t speed_factor = f3d::ratio_t{1.0};
    } animation;

    struct camera {
      std::optional<int> index;
      std::optional<bool> orthographic;
    } camera;

    std::optional<std::string> force_reader;
    f3d::direction_t up_direction = f3d::direction_t{0,1,0};
  } scene;

  struct ui {
    bool animation_progress = false;
    bool axis = false;
    bool cheatsheet = false;
    bool console = false;
    bool dropzone = false;
    std::string dropzone_info = "";
    bool filename = false;
    std::string filename_info = "";
    std::optional<std::filesystem::path> font_file;
    bool fps = false;
    bool loader_progress = false;
    bool metadata = false;
    bool minimal_console = false;
    bool scalar_bar = false;
    f3d::ratio_t scale = f3d::ratio_t{1.0};
  } ui;


  // clang-format on
};

}

// Certain options types are not trivially streamable
//----------------------------------------------------------------------------
inline std::ostream& operator<<(std::ostream& os, const f3d::color_t& color)
{
  os << f3d::options::format(color);
  return os;
}
inline std::ostream& operator<<(std::ostream& os, const f3d::direction_t& direction)
{
  os << f3d::options::format(direction);
  return os;
}
inline std::ostream& operator<<(std::ostream& os, const f3d::colormap_t& cm)
{
  os << f3d::options::format(cm);
  return os;
}
inline std::ostream& operator<<(std::ostream& os, const f3d::transform2d_t& transform)
{
  os << f3d::options::format(transform);
  return os;
}
#endif
