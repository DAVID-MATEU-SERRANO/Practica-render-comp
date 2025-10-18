#include "../include/parse_config.hpp"
#include "../include/config.hpp"
#include "../include/util.hpp"

#include <array>
#include <string>
#include <string_view>

using parse::util::expect_positive;
using parse::util::parse_error;
using parse::util::parse_three_doubles;
using parse::util::strip_comment_and_trim;
using parse::util::to_double;
using parse::util::to_int;
using parse::util::to_uint64;
using parse::util::trim;
using parse::util::validate_rgb;

namespace {  // ----------- helpers "privados" al .cpp -----------

  bool handle_image(std::string_view key, std::string_view val, std::size_t ln, Config & cfg) {
    if (key == "aspect_ratio_width") {
      cfg.aspect_ratio_width = to_int(std::string(val), ln, "aspect_ratio_width");
      expect_positive(cfg.aspect_ratio_width, ln, "aspect_ratio_width");
      return true;
    }
    if (key == "aspect_ratio_height") {
      cfg.aspect_ratio_height = to_int(std::string(val), ln, "aspect_ratio_height");
      expect_positive(cfg.aspect_ratio_height, ln, "aspect_ratio_height");
      return true;
    }
    if (key == "image_width") {
      cfg.image_width = to_int(std::string(val), ln, "image_width");
      expect_positive(cfg.image_width, ln, "image_width");
      return true;
    }
    return false;
  }

  bool handle_camera(std::string_view key, std::string_view val, std::size_t ln, Config & cfg) {
    std::array<double, 3> v{};
    if (key == "camera_position") {
      parse_three_doubles(std::string(val), v, ln, "camera_position");
      cfg.camera_position = v;
      return true;
    }
    if (key == "camera_target") {
      parse_three_doubles(std::string(val), v, ln, "camera_target");
      cfg.camera_target = v;
      return true;
    }
    if (key == "camera_up" or key == "camera_north") {
      parse_three_doubles(std::string(val), v, ln, "camera_up");
      cfg.camera_up = v;
      return true;
    }
    if (key == "fov_deg") {
      cfg.fov_deg = to_double(std::string(val), ln, "fov_deg");
      if (cfg.fov_deg <= 0.0 or cfg.fov_deg >= 180.0) {
        parse_error(ln, "fov_deg debe estar en (0, 180)");
      }
      return true;
    }
    return false;
  }

  bool handle_render(std::string_view key, std::string_view val, std::size_t ln, Config & cfg) {
    if (key == "samples_per_pixel") {
      cfg.samples_per_pixel = to_int(std::string(val), ln, "samples_per_pixel");
      expect_positive(cfg.samples_per_pixel, ln, "samples_per_pixel");
      return true;
    }
    if (key == "max_depth") {
      cfg.max_depth = to_int(std::string(val), ln, "max_depth");
      expect_positive(cfg.max_depth, ln, "max_depth");
      return true;
    }
    if (key == "gamma") {
      cfg.gamma = to_double(std::string(val), ln, "gamma");
      if (cfg.gamma <= 0.0) {
        parse_error(ln, "gamma debe ser > 0");
      }
      return true;
    }
    return false;
  }

  bool handle_background(std::string_view key, std::string_view val, std::size_t ln, Config & cfg) {
    std::array<double, 3> v{};
    if (key == "background_dark" or key == "bg_dark") {
      parse_three_doubles(std::string(val), v, ln, "background_dark");
      validate_rgb(v[0], v[1], v[2], ln);
      cfg.bg_dark = v;
      return true;
    }
    if (key == "background_light" or key == "bg_light") {
      parse_three_doubles(std::string(val), v, ln, "background_light");
      validate_rgb(v[0], v[1], v[2], ln);
      cfg.bg_light = v;
      return true;
    }
    return false;
  }

  bool handle_seeds(std::string_view key, std::string_view val, std::size_t ln, Config & cfg) {
    if (key == "material_seed") {
      cfg.material_seed = to_uint64(std::string(val), ln, "material_seed");
      return true;
    }
    if (key == "ray_seed") {
      cfg.ray_seed = to_uint64(std::string(val), ln, "ray_seed");
      return true;
    }
    return false;
  }

}  // namespace

namespace parse2 {

  void parse_config_stream(std::istream & in, Config & cfg) {
    std::string line;
    std::size_t lineno = 0;

    while (std::getline(in, line)) {
      ++lineno;
      line = strip_comment_and_trim(line);
      if (line.empty()) {
        continue;
      }

      auto eq = line.find('=');
      if (eq == std::string::npos) {
        parse_error(lineno, std::string("Esperaba 'key = value', got: \"") + line + "\"");
      }
      std::string key = trim(line.substr(0, eq));
      std::string val = trim(line.substr(eq + 1));

      bool handled = false;
      handled      = handle_image(key, val, lineno, cfg) or handled;
      handled      = handle_camera(key, val, lineno, cfg) or handled;
      handled      = handle_render(key, val, lineno, cfg) or handled;
      handled      = handle_background(key, val, lineno, cfg) or handled;
      handled      = handle_seeds(key, val, lineno, cfg) or handled;

      if (!handled) {
        parse_error(lineno, "Clave desconocida: " + key);
      }
    }

    if (!cfg.is_valid()) {
      throw std::runtime_error("Config invalida tras parseo (revisa rangos y campos)");
    }
  }

}  // namespace parse2
