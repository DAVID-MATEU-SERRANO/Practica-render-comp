#include "../include/parse_scene.hpp"
#include "../include/color.hpp"
#include "../include/cylinder.hpp"
#include "../include/matte.hpp"
#include "../include/metal.hpp"
#include "../include/parse_exception.hpp"
#include "../include/point.hpp"
#include "../include/refractive.hpp"
#include "../include/sphere.hpp"
#include "../include/util.hpp"
#include "../include/vector.hpp"
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace render { namespace {

  // Función para obtener material del index - CORREGIDA
  template <typename T> T get_material_from_code(render::Scene & scene, size_t code) {
    // Obtenemos índice y tipo
    std::size_t const material_array_index = code / 10;
    int const material_type                = static_cast<int>(code % 10);

    if constexpr (std::is_same_v<T, render::Matte>) {
      if (material_type != 0) {
        throw std::runtime_error("Error obtaining material. Case Matte");
      }
      // Verificar que el índice es válido
      auto const & mattes = scene.get_mattes();
      if (material_array_index >= mattes.size()) {
        throw std::runtime_error("Matte material index out of range");
      }
      return mattes.at(material_array_index);
    } else if constexpr (std::is_same_v<T, render::Metal>) {
      if (material_type != 1) {
        throw std::runtime_error("Error obtaining material. Case Metal");
      }
      // Verificar que el índice es válido
      auto const & metals = scene.get_metals();
      if (material_array_index >= metals.size()) {
        throw std::runtime_error("Metal material index out of range");
      }
      return metals.at(material_array_index);
    } else if constexpr (std::is_same_v<T, render::Refractive>) {
      if (material_type != 2) {
        throw std::runtime_error("Error obtaining material. Case refractive");
      }
      // Verificar que el índice es válido
      auto const & refractives = scene.get_refractives();
      if (material_array_index >= refractives.size()) {
        throw std::runtime_error("Refractive material index out of range");
      }
      return refractives.at(material_array_index);
    } else {
      throw std::runtime_error("Obtained not recognized material");
    }
  }

  void parse_matte_line(std::vector<std::string> const & tokens, Scene & scene,
                        std::string const & line_content) {
    parse::util::expect_token_count(tokens, 4, line_content, "matte");

    double const r = parse::util::to_double(tokens[1]);
    double const g = parse::util::to_double(tokens[2]);
    double const b = parse::util::to_double(tokens[3]);

    Color const reflectance(r, g, b);
    render::Matte const matte(tokens[0], reflectance);
    scene.add_material_matte(matte, line_content);
  }

  void parse_metal_line(std::vector<std::string> const & tokens, Scene & scene,
                        std::string const & line_content) {
    parse::util::expect_token_count(tokens, 5, line_content, "metal");

    double const r         = parse::util::to_double(tokens[1]);
    double const g         = parse::util::to_double(tokens[2]);
    double const b         = parse::util::to_double(tokens[3]);
    double const diffusion = parse::util::to_double(tokens[4]);

    Color const reflectance(r, g, b);

    if (diffusion < 0.0) {
      parse::throw_invalid_parameters("metal", line_content);
    }

    render::Metal const metal(tokens[0], reflectance, diffusion);
    scene.add_material_metal(metal, line_content);
  }

  void parse_refractive_line(std::vector<std::string> const & tokens, Scene & scene,
                             std::string const & line_content) {
    parse::util::expect_token_count(tokens, 2, line_content, "refractive");

    double const refraction_index = parse::util::to_double(tokens[1]);

    if (refraction_index <= 0.0) {
      parse::throw_invalid_parameters("refractive", line_content);
    }

    render::Refractive const refractive(tokens[0], refraction_index);
    scene.add_material_refractive(refractive, line_content);
  }

  void parse_sphere_line(std::vector<std::string> const & tokens, Scene & scene,
                         std::string const & line_content) {
    parse::util::expect_token_count(tokens, 5, line_content, "sphere");

    double const cx                   = parse::util::to_double(tokens[0]);
    double const cy                   = parse::util::to_double(tokens[1]);
    double const cz                   = parse::util::to_double(tokens[2]);
    double const radius               = parse::util::to_double(tokens[3]);
    std::string const & material_name = tokens[4];

    if (radius < 0.0) {
      parse::throw_invalid_parameters("sphere", line_content);
    }

    auto & material_array_index = scene.get_material_index();
    auto it                     = material_array_index.find(material_name);
    if (it == material_array_index.end()) {
      parse::throw_material_not_found(material_name, line_content);
    }

    // Obtener material
    std::size_t const code  = it->second;
    int const material_type = static_cast<int>(code % 10);

    render::Point const center(cx, cy, cz);

    if (material_type == 0) {
      auto material = get_material_from_code<render::Matte>(scene, code);
      render::Sphere const s(center, radius, material);
      scene.add_sphere(s);
    } else if (material_type == 1) {
      auto material = get_material_from_code<render::Metal>(scene, code);
      render::Sphere const s(center, radius, material);
      scene.add_sphere(s);
    } else {
      auto material = get_material_from_code<render::Refractive>(scene, code);
      render::Sphere const s(center, radius, material);
      scene.add_sphere(s);
    }
  }

  struct CylinderParams {
    render::Point center;
    render::Vector direction;
    double radius;
    std::string material_name;
  };

  CylinderParams parse_cylinder_geometry(std::vector<std::string> const & t,
                                         std::string const & lineforprint) {
    double const cx                   = parse::util::to_double(t[0]);
    double const cy                   = parse::util::to_double(t[1]);
    double const cz                   = parse::util::to_double(t[2]);
    double const radius               = parse::util::to_double(t[3]);
    double const ax                   = parse::util::to_double(t[4]);
    double const ay                   = parse::util::to_double(t[5]);
    double const az                   = parse::util::to_double(t[6]);
    std::string const & material_name = t[7];

    if (radius <= 0.0) {
      parse::throw_invalid_parameters("cylinder", lineforprint);
    }

    if (ax == 0.0 and ay == 0.0 and az == 0.0) {
      parse::throw_invalid_parameters("cylinder (null axis)", lineforprint);
    }

    return {render::Point(cx, cy, cz), render::Vector(ax, ay, az), radius, material_name};
  }

  void parse_cylinder_line(std::vector<std::string> const & tokens, render::Scene & scene,
                           std::string const & line_content) {
    parse::util::expect_token_count(tokens, 8, line_content, "cylinder");

    CylinderParams const params = parse_cylinder_geometry(tokens, line_content);

    auto & material_array_index = scene.get_material_index();
    auto it                     = material_array_index.find(params.material_name);

    if (it == material_array_index.end()) {
      parse::throw_material_not_found(params.material_name, line_content);
    }

    std::size_t const code  = it->second;
    int const material_type = static_cast<int>(code % 10);

    if (material_type == 0) {
      auto material = get_material_from_code<render::Matte>(scene, code);
      render::Cylinder const c(params.center, params.radius, params.direction, material);
      scene.add_cylinder(c);
    } else if (material_type == 1) {
      auto material = get_material_from_code<render::Metal>(scene, code);
      render::Cylinder const c(params.center, params.radius, params.direction, material);
      scene.add_cylinder(c);
    } else {
      auto material = get_material_from_code<render::Refractive>(scene, code);
      render::Cylinder const c(params.center, params.radius, params.direction, material);
      scene.add_cylinder(c);
    }
  }

  void dispatch_scene_entity(std::string const & tag, std::vector<std::string> const & tokens,
                             Scene & scene, std::string const & line_content) {
    if (tag == "matte") {
      parse_matte_line(tokens, scene, line_content);
    } else if (tag == "metal") {
      parse_metal_line(tokens, scene, line_content);
    } else if (tag == "refractive") {
      parse_refractive_line(tokens, scene, line_content);
    } else if (tag == "sphere") {
      parse_sphere_line(tokens, scene, line_content);
    } else if (tag == "cylinder") {
      parse_cylinder_line(tokens, scene, line_content);
    } else {
      std::ostringstream oss;
      oss << "Error: Unknown scene entity: " << tag;
      throw std::runtime_error(oss.str());
    }
  }

}}  // namespace render

namespace parse {

  void parse_scene_stream(std::istream & in, render::Scene & scene) {
    std::string line;
    std::regex const scene_line_regex(R"(\s*([a-z]+):\s*(.*))");
    std::smatch match;

    while (std::getline(in, line)) {
      std::string const & line_content = line;

      // Eliminar comentarios
      if (auto p = line.find('#'); p != std::string::npos) {
        line.erase(p);
      }
      line = util::trim(line);
      if (line.empty()) {
        continue;
      }

      try {
        if (std::regex_match(line, match, scene_line_regex)) {
          std::string const tag      = match[1].str();
          std::string const args_raw = match[2].str();
          std::string const args     = util::trim(args_raw);
          auto tokens                = util::split_ws(args);

          render::dispatch_scene_entity(tag, tokens, scene, line_content);
        } else {
          std::ostringstream oss;
          oss << "Error: Unknown scene entity: " << line;
          throw std::runtime_error(oss.str());
        }
      } catch (ParseException const & e) {
        std::cerr << "Error: " << e.what() << "\n"
                  << "Line: \"" << e.get_line_content() << "\"\n";
        exit(EXIT_FAILURE);
      } catch (std::runtime_error const & e) {
        std::cerr << "Error: " << e.what() << "\n";
        exit(EXIT_FAILURE);
      }
    }
  }

}  // namespace parse
