#include "../include/parse_config.hpp"
#include "../include/color.hpp"
#include "../include/point.hpp"
#include "../include/pov.hpp"
#include "../include/scene.hpp"
#include "../include/util.hpp"
#include "../include/vector.hpp"

#include <array>
#include <cctype>
#include <cstddef>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>

using parse::util::expect_positive;
using parse::util::parse_three_doubles;
using parse::util::strip_comment_and_trim;
using parse::util::to_double_config;
using parse::util::to_int;
using parse::util::to_uint64;
using parse::util::validate_rgb_config;

namespace {  // ----------- helpers "privados"-----------

  // FUNCION PARA ASEGURAR EL FORMATO DE LOS 2 ULTIMOS TIPOS DE ERRORES
  inline void ensure_token_count_exact(std::string_view val, std::size_t expected,
                                       std::string const & lineforprint, std::string const & key) {
    std::string s{val};
    size_t i = 0, n = s.size();
    auto is_space = [](char c) -> bool { return std::isspace(static_cast<unsigned char>(c)) != 0; };

    // avanzar consumiendo 'expected' tokens
    std::size_t tokens = 0;
    while (i < n and tokens < expected) {
      while (i < n and (std::isspace(static_cast<unsigned char>(s[i])) != 0)) {
        ++i;  // saltar espacios
      }
      if (i >= n) {
        break;
      }
      while (i < n and !is_space(s[i])) {
        ++i;  // consumir token
      }
      ++tokens;
    }

    if (tokens < expected) {
      std::ostringstream oss;
      oss << "Invalid value for key: " << "[" << key << "]" << "\n"
          << "Line: \"" << lineforprint << "\"";
      throw std::runtime_error(oss.str());
    }

    // posición donde empezarían los datos extra (si existen)
    size_t j = i;
    while (j < n and is_space(s[j])) {
      ++j;  // saltar separación
    }
    if (j < n) {
      std::string const extra = s.substr(j);  // <-- restarle a la línea la posición j
      std::ostringstream oss;
      oss << "Extra data after configuration value for key:" << " " << "[" << key << "]" << "\n"
          << "Extra: \"" << extra << "\"\n";
      throw std::runtime_error(oss.str());
    }
  }

  std::tuple<bool, int, int> handle_image_aspect_ratio(std::string_view key, std::string_view val,
                                                       std::string const & lineforprint) {
    int w = 16, h = 9;
    if (key == "aspect_ratio") {
      ensure_token_count_exact(val, 2, lineforprint, "aspect_ratio");
      std::istringstream iss{std::string(val)};

      if (!(iss >> w >> h)) {
        std::ostringstream oss;
        oss << "Invalid value for key: \"" << "[" << key << ":" << "]\"" << "\n"
            << "Line: \"" << lineforprint << "\"";
        throw std::runtime_error(oss.str());
      }
      if (w <= 0 or h <= 0) {
        std::ostringstream oss;
        oss << "Invalid value for key: \"" << "[" << key << ":" << "]\"" << "\n"
            << "Line: \"" << lineforprint << "\"";
        throw std::runtime_error(oss.str());
      }
      return {true, w, h};
    }
    return {false, w, h};
  }

  std::pair<bool, int> handle_image_width(std::string_view key, std::string_view val,
                                          std::string const & lineforprint) {
    int w = 1'920;
    if (key == "image_width") {
      ensure_token_count_exact(val, 1, lineforprint, "image_width");
      // Convert the value to int, validate and store into cfg
      w = to_int(std::string(val), lineforprint, "image_width");
      expect_positive(w, lineforprint, "image_width");
      return {true, w};
    }
    return {false, w};
  }

  bool handle_camera_position(std::string_view key, std::string_view val,
                              std::string const & lineforprint, render::Pov & pov) {
    std::array<double, 3> v{};
    if (key == "camera_position") {
      ensure_token_count_exact(val, 3, lineforprint, "camera_position");
      parse_three_doubles(std::string(val), v, lineforprint, "camera_position");
      render::Point const position(v[0], v[1], v[2]);
      pov.set_camera_position(position);
      return true;
    }

    return false;
  }

  bool handle_camera_target(std::string_view key, std::string_view val,
                            std::string const & lineforprint, render::Pov & pov) {
    std::array<double, 3> v{};
    if (key == "camera_target") {
      ensure_token_count_exact(val, 3, lineforprint, "camera_target");
      parse_three_doubles(std::string(val), v, lineforprint, "camera_target");
      render::Point const target(v[0], v[1], v[2]);
      pov.set_camera_target(target);
      return true;
    }
    return false;
  }

  bool handle_north(std::string_view key, std::string_view val, std::string const & lineforprint,
                    render::Pov & pov) {
    std::array<double, 3> v{};
    if (key == "camera_north") {
      ensure_token_count_exact(val, 3, lineforprint, "camera_north");
      parse_three_doubles(std::string(val), v, lineforprint, "camera_north");
      render::Vector const north(v[0], v[1], v[2]);
      pov.set_camera_north(north);
      return true;
    }
    return false;
  }

  bool handle_fov(std::string_view key, std::string_view val, std::string const & lineforprint,
                  render::Pov & pov) {
    double fov = 90.0;
    if (key == "field_of_view") {
      ensure_token_count_exact(val, 1, lineforprint, "field_of_view");
      fov = to_double_config(std::string(val), lineforprint, "field_of_view");
      if (fov <= 0.0 or fov >= 180.0) {
        std::ostringstream oss;
        oss << "Invalid value for key: \"" << "[" << key << ":" << "]\"" << "\n"
            << "Line: \"" << lineforprint << "\"";
        throw std::runtime_error(oss.str());
      }
      pov.set_field_of_view(fov);
      return true;
    }
    return false;
  }

  bool handle_render(std::string_view key, std::string_view val, std::string const & lineforprint,
                     render::Scene & scene) {
    if (key == "samples_per_pixel") {
      ensure_token_count_exact(val, 1, lineforprint, "samples_per_pixel");
      int const samples_per_pixel = to_int(std::string(val), lineforprint, "samples_per_pixel");
      expect_positive(samples_per_pixel, lineforprint, "samples_per_pixel");
      scene.set_samples_per_pixel(samples_per_pixel);
      return true;
    }

    if (key == "max_depth") {
      ensure_token_count_exact(val, 1, lineforprint, "max_depth");
      int const max_depth = to_int(std::string(val), lineforprint, "max_depth");
      expect_positive(max_depth, lineforprint, "max_depth");
      scene.set_max_depth(max_depth);
      return true;
    }

    if (key == "gamma") {
      ensure_token_count_exact(val, 1, lineforprint, "gamma");
      double const gamma = to_double_config(std::string(val), lineforprint, "gamma");
      if (gamma <= 0.0) {
        std::ostringstream oss;
        oss << "Invalid value for key: \"" << "[" << key << ":" << "]\"" << "\n"
            << "Line: \"" << lineforprint << "\"";
        throw std::runtime_error(oss.str());
      }
      scene.set_gamma(gamma);
      return true;
    }
    return false;
  }

  bool handle_background(std::string_view key, std::string_view val,
                         std::string const & lineforprint, render::Scene & scene) {
    std::array<double, 3> colors{};
    if (key == "background_dark_color") {
      ensure_token_count_exact(val, 3, lineforprint, "background_dark_color");
      parse_three_doubles(std::string(val), colors, lineforprint, "background_dark");
      validate_rgb_config(colors, lineforprint, key);
      render::Color const color(colors[0], colors[1], colors[2]);
      scene.set_background_dark_color(color);
      return true;
    }
    if (key == "background_light_color") {
      ensure_token_count_exact(val, 3, lineforprint, "background_light_color");
      parse_three_doubles(std::string(val), colors, lineforprint, "background_light_color");
      validate_rgb_config(colors, lineforprint, key);
      render::Color const color(colors[0], colors[1], colors[2]);
      scene.set_background_light_color(color);
      return true;
    }
    return false;
  }

  bool handle_seeds(std::string_view key, std::string_view val, std::string const & lineforprint,
                    render::Scene & scene) {
    if (key == "material_rng_seed") {
      ensure_token_count_exact(val, 1, lineforprint, "material_rng_seed");
      scene.set_material_rng_seed(to_uint64(std::string(val), lineforprint, "material_rng_seed"));
      return true;
    }
    if (key == "ray_rng_seed") {
      ensure_token_count_exact(val, 1, lineforprint, "ray_rng_seed");
      scene.set_rays_rng_seed(to_uint64(std::string(val), lineforprint, "ray_rng_seed"));
      return true;
    }
    return false;
  }

}  // namespace

namespace parse {

  void parse_config_stream(std::istream & in, render::Scene & scene) {
    std::string line;
    std::string lineforprint;

    render::Pov pov;
    pov.set_camera_position(render::Point(0, 0, -10));
    pov.set_camera_target(render::Point(0, 0, 0));
    pov.set_camera_north(render::Vector(0, 1, 0));
    pov.set_field_of_view(90.0);

    int parsed_image_width = 1'920;
    int parsed_ar_w        = 16;
    int parsed_ar_h        = 9;

    scene.set_samples_per_pixel(20);
    scene.set_max_depth(5);
    scene.set_gamma(2.2);
    scene.set_material_rng_seed(13);
    scene.set_rays_rng_seed(19);
    scene.set_background_dark_color(render::Color(0.25, 0.5, 1));
    scene.set_background_light_color(render::Color(1, 1, 1));

    std::regex const config_line_regex(R"(^\s*([A-Za-z_]+):\s*(.*?)\s*$)");
    std::smatch match;

    while (std::getline(in, line)) {
      lineforprint = line;
      line         = strip_comment_and_trim(line);
      if (line.empty()) {
        continue;
      }

      try {
        std::string key;
        std::string val;

        if (std::regex_match(line, match, config_line_regex)) {
          key = match[1].str();
          val = match[2].str();
        } else {
          std::ostringstream oss;
          oss << "Invalid configuration line format." << "\n"
              << "Line: \"" << lineforprint << "\"";
          throw std::runtime_error(oss.str());
        }

        bool handled         = false;
        auto [ok_ar, aw, ah] = handle_image_aspect_ratio(key, val, lineforprint);
        if (ok_ar) {
          parsed_ar_w = aw;
          parsed_ar_h = ah;
          handled     = true;
        }

        auto [ok_w, w] = handle_image_width(key, val, lineforprint);
        if (ok_w) {
          parsed_image_width = w;
          handled            = true;
        }

        handled = handle_camera_position(key, val, lineforprint, pov) or handled;
        handled = handle_camera_target(key, val, lineforprint, pov) or handled;
        handled = handle_fov(key, val, lineforprint, pov) or handled;
        handled = handle_north(key, val, lineforprint, pov) or handled;

        handled = handle_render(key, val, lineforprint, scene) or handled;
        handled = handle_background(key, val, lineforprint, scene) or handled;
        handled = handle_seeds(key, val, lineforprint, scene) or handled;

        if (!handled) {
          std::ostringstream oss;
          oss << "Unknown configuration key: \"" << "[" << key << ":" << "]\"" << "\n";
          throw std::runtime_error(oss.str());
        }
      } catch (std::runtime_error const & e) {
        throw;  // Re-throw to be handled by caller (same behaviour as before)
      }
    }

    render::ImageSize const isz =
        render::Pov::compute_image_size(parsed_image_width, parsed_ar_w, parsed_ar_h);
    // aquí puedes usar `isz` para construir el Pov o asignarlo según tu diseño
    // ej: pov = render::Pov(position, target, north, cfg.field_of_view, isz);
    pov.set_image_size(isz);
    scene.set_pov(pov);
  }

}  // namespace parse
