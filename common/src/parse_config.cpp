#include "parse_config.hpp"
#include "config.hpp"
#include "util.hpp"

#include <array>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>

using parse::util::expect_positive;
using parse::util::parse_three_doubles;
using parse::util::strip_comment_and_trim;
using parse::util::to_double_config;
using parse::util::to_int;
using parse::util::to_uint64;
using parse::util::trim;
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
      std::string extra = s.substr(j);  // <-- restarle a la línea la posición j
      std::ostringstream oss;
      oss << "Extra data after configuration value for key:" << " " << "[" << key << "]" << "\n"
          << "Extra: \"" << extra << "\"\n";
      throw std::runtime_error(oss.str());
    }
  }

  bool handle_image(std::string_view key, std::string_view val, std::string const & lineforprint,
                    Config & cfg) {
    if (key == "aspect_ratio") {
      ensure_token_count_exact(val, 2, lineforprint, "aspect_ratio");
      std::istringstream iss{std::string(val)};
      int w = 0, h = 0;
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
      cfg.aspect_ratio_width  = w;
      cfg.aspect_ratio_height = h;
      return true;
    }
    if (key == "image_width") {
      ensure_token_count_exact(val, 1, lineforprint, "image_width");
      cfg.image_width = to_int(std::string(val), lineforprint, "image_width");
      expect_positive(cfg.image_width, lineforprint, "image_width");
      return true;
    }
    return false;
  }

  bool handle_camera(std::string_view key, std::string_view val, std::string const & lineforprint,
                     Config & cfg) {
    std::array<double, 3> v{};
    if (key == "camera_position") {
      ensure_token_count_exact(val, 3, lineforprint, "camera_position");
      parse_three_doubles(std::string(val), v, lineforprint, "camera_position");
      cfg.camera_position = v;
      return true;
    }
    if (key == "camera_target") {
      ensure_token_count_exact(val, 3, lineforprint, "camera_target");
      parse_three_doubles(std::string(val), v, lineforprint, "camera_target");
      cfg.camera_target = v;
      return true;
    }
    if (key == "camera_north") {
      ensure_token_count_exact(val, 3, lineforprint, "camera_north");
      parse_three_doubles(std::string(val), v, lineforprint, "camera_north");
      cfg.camera_north = v;
      return true;
    }
    if (key == "field_of_view") {
      ensure_token_count_exact(val, 1, lineforprint, "field_of_view");
      cfg.field_of_view = to_double_config(std::string(val), lineforprint, "field_of_view");
      if (cfg.field_of_view <= 0.0 or cfg.field_of_view >= 180.0) {
        std::ostringstream oss;
        oss << "Invalid value for key: \"" << "[" << key << ":" << "]\"" << "\n"
            << "Line: \"" << lineforprint << "\"";
        throw std::runtime_error(oss.str());
      }
      return true;
    }
    return false;
  }

  bool handle_render(std::string_view key, std::string_view val, std::string const & lineforprint,
                     Config & cfg) {
    if (key == "samples_per_pixel") {
      ensure_token_count_exact(val, 1, lineforprint, "samples_per_pixel");
      cfg.samples_per_pixel = to_int(std::string(val), lineforprint, "samples_per_pixel");
      expect_positive(cfg.samples_per_pixel, lineforprint, "samples_per_pixel");
      return true;
    }
    if (key == "max_depth") {
      ensure_token_count_exact(val, 1, lineforprint, "max_depth");
      cfg.max_depth = to_int(std::string(val), lineforprint, "max_depth");
      expect_positive(cfg.max_depth, lineforprint, "max_depth");
      return true;
    }
    if (key == "gamma") {
      ensure_token_count_exact(val, 1, lineforprint, "gamma");
      cfg.gamma = to_double_config(std::string(val), lineforprint, "gamma");
      if (cfg.gamma <= 0.0) {
        std::ostringstream oss;
        oss << "Invalid value for key: \"" << "[" << key << ":" << "]\"" << "\n"
            << "Line: \"" << lineforprint << "\"";
        throw std::runtime_error(oss.str());
      }
      return true;
    }
    return false;
  }

  bool handle_background(std::string_view key, std::string_view val,
                         std::string const & lineforprint, Config & cfg) {
    std::array<double, 3> colors{};
    if (key == "background_dark_color") {
      ensure_token_count_exact(val, 3, lineforprint, "background_dark_color");
      parse_three_doubles(std::string(val), colors, lineforprint, "background_dark");
      validate_rgb_config(colors, lineforprint, key);
      cfg.background_dark_color = colors;
      return true;
    }
    if (key == "background_light_color") {
      ensure_token_count_exact(val, 3, lineforprint, "background_light_color");
      parse_three_doubles(std::string(val), colors, lineforprint, "background_light_color");
      validate_rgb_config(colors, lineforprint, key);
      cfg.background_light_color = colors;
      return true;
    }
    return false;
  }

  bool handle_seeds(std::string_view key, std::string_view val, std::string const & lineforprint,
                    Config & cfg) {
    if (key == "material_rng_seed") {
      ensure_token_count_exact(val, 1, lineforprint, "material_rng_seed");
      cfg.material_rng_seed = to_uint64(std::string(val), lineforprint, "material_rng_seed");
      return true;
    }
    if (key == "ray_rng_seed") {
      ensure_token_count_exact(val, 1, lineforprint, "ray_rng_seed");
      cfg.ray_rng_seed = to_uint64(std::string(val), lineforprint, "ray_rng_seed");
      return true;
    }
    return false;
  }

}  // namespace

namespace parse2 {

  void parse_config_stream(std::istream & in, Config & cfg) {
    std::string line;
    std::string lineforprint;

    while (std::getline(in, line)) {
      lineforprint = line;
      line         = strip_comment_and_trim(line);
      if (line.empty()) {
        continue;
      }

      auto eq = line.find(':');
      if (eq == std::string::npos) {
        std::string key = trim(line);
        std::ostringstream oss;
        oss << "Unknown configuration key: \"" << "[" << key + ":" << "]\"" << "\n";
        throw std::runtime_error(oss.str());
      }
      std::string key = trim(line.substr(0, eq));
      std::string val = trim(line.substr(eq + 1));

      bool handled = false;
      handled      = handle_image(key, val, lineforprint, cfg) or handled;
      handled      = handle_camera(key, val, lineforprint, cfg) or handled;
      handled      = handle_render(key, val, lineforprint, cfg) or handled;
      handled      = handle_background(key, val, lineforprint, cfg) or handled;
      handled      = handle_seeds(key, val, lineforprint, cfg) or handled;

      if (!handled) {
        std::ostringstream oss;
        oss << "Unknown configuration key: \"" << "[" << key + ":" << "]\"" << "\n";
        throw std::runtime_error(oss.str());
      }
    }

    if (!cfg.is_valid()) {
      throw std::runtime_error("Config invalida tras parseo (revisa rangos y campos) ESTE PRINT ES "
                               "MIO NO LO PIDE EL PROFESOR");
    }
  }

}  // namespace parse2
