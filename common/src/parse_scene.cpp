#include "parse_scene.hpp"
#include "point.hpp"
#include "scene.hpp"
#include "util.hpp"

#include "cylinder.hpp"
#include "metal.hpp"
#include "refractive.hpp"
#include "sphere.hpp"
#include "vector.hpp"
#include <array>
#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <istream>
#include <ostream>
#include <vector>

using namespace parse::util;

// ============================ Parsers por etiqueta ============================

namespace {

  void parse_matte_line(std::vector<std::string> const & t, Scene & scene,
                        std::string const & lineforprint) {
    // matte: <name> r g b
    expect_token_count(t, 4, lineforprint, "matte");

    double r = to_double(t[1], lineforprint, "matte");
    double g = to_double(t[2], lineforprint, "matte");
    double b = to_double(t[3], lineforprint, "matte");

    render::Matte m(t[0], r, g, b);

    std::array<double, 3> color = {r, g, b};

    validate_rgb(color, lineforprint, "matte");

    scene.add_material_Matte(m, lineforprint);
  }

  void parse_metal_line(std::vector<std::string> const & t, Scene & scene,
                        std::string const & lineforprint) {
    // metal: <name> r g b roughness
    expect_token_count(t, 5, lineforprint, "metal");

    double r                    = to_double(t[1], lineforprint, "metal");
    double g                    = to_double(t[2], lineforprint, "metal");
    double b                    = to_double(t[3], lineforprint, "metal");
    double rough                = to_double(t[4], lineforprint, "metal");
    std::array<double, 3> color = {r, g, b};
    validate_rgb(color, lineforprint, "metal");

    if (rough < 0.0 or rough > 1.0) {
      std::ostringstream oss;
      oss << "Roughness must be in [0.0, 1.0], got: " << rough;
      throw std::runtime_error(oss.str());
    }
    render::Metal m(t[0], r, g, b, rough);
    scene.add_material_Metal(m, lineforprint);
  }

  void parse_refractive_line(std::vector<std::string> const & t, Scene & scene,
                             std::string const & lineforprint) {
    // refractive: <name> ior
    expect_token_count(t, 2, lineforprint, "refractive");

    double ior = to_double(t[1], "ior", "refractive");
    if (ior <= 1.0) {
      std::ostringstream oss;
      oss << "Invalid" << " " << "refractive" << " parameters" << "\n"
          << "Line: \"" << lineforprint << "\"";
      throw std::runtime_error(oss.str());
    }

    render::Refractive m(t[0], ior);
    scene.add_material_Refractive(m, lineforprint);
  }

  void parse_sphere_line(std::vector<std::string> const & t, Scene & scene,
                         std::string const & lineforprint) {
    // sphere: cx cy cz radius material_name
    expect_token_count(t, 5, lineforprint, "sphere");

    render::Point center(to_double(t[0], lineforprint, "sphere"),
                         to_double(t[1], lineforprint, "sphere"),
                         to_double(t[2], lineforprint, "sphere"));

    double radius = to_double(t[3], lineforprint, "sphere");

    if (radius <= 0.0) {
      std::ostringstream oss;
      oss << "Invalid" << " " << "sphere" << " parameters" << "\n"
          << "Line: \"" << lineforprint << "\"";
      throw std::runtime_error(oss.str());
    }

    // Verificar que el material exista PEDIDO EN EL ENUNCIADO
    if (scene.material_index.find(t[4]) == scene.material_index.end()) {
      std::ostringstream oss;
      oss << "Material not found: \"" << "[" << t[4] << "]\"\n"
          << "Line: \"" << lineforprint << "\"";
      throw std::runtime_error(oss.str());
    }

    auto it = scene.material_index.find(t[4]);
    if (it != scene.material_index.end()) {
      // it es un iterador; it->first = clave (string), it->second = valor (size_t)
      std::size_t code    = it->second;
      int n               = static_cast<int>(code);
      int ultimo          = n % 10;  // Valor ficticio para evitar warning de variable no usada
      std::size_t primero = 0;
      while (n >= 10) {
        n /= 10;
        primero = static_cast<std::size_t>(n);
      }

      if (ultimo == 0) {  // SIGNIFICA QUE EL VECTOR DEL MATERIAL ES EL DE MATT
        render::Sphere s(center, radius, scene.mattes[primero]);
        scene.add_sphere(s);
      }
      if (ultimo == 1) {  // SIGNIFICA QUE EL VECTOR DEL MATERIAL ES EL DE METAL
        render::Sphere s(center, radius, scene.metales[primero]);
        scene.add_sphere(s);
      }
      if (ultimo == 2) {  // SIGNIFICA QUE EL VECTOR DEL MATERIAL ES EL DE REFRACTARIO
        render::Sphere s(center, radius, scene.refractarios[primero]);
        scene.add_sphere(s);
      }
    }
  }

  void parse_cylinder_line(std::vector<std::string> const & t, Scene & scene,
                           std::string const & lineforprint) {
    // cylinder: bx by bz ax ay az radius material_name
    expect_token_count(t, 8, lineforprint, "cylinder");

    render::Point center(to_double(t[0], lineforprint, "cylinder"),
                         to_double(t[1], lineforprint, "cylinder"),
                         to_double(t[2], lineforprint, "cylinder"));
    render::Vector direccion(to_double(t[3], lineforprint, "cylinder"),
                             to_double(t[4], lineforprint, "cylinder"),
                             to_double(t[5], lineforprint, "cylinder"));
    double radius = to_double(t[6], lineforprint, "cylinder");
    if (radius <= 0.0) {
      std::ostringstream oss;
      oss << "Invalid" << " " << "cylinder" << " parameters" << "\n"
          << "Line: \"" << lineforprint << "\"";
      throw std::runtime_error(oss.str());
    }
    std::array<double, 3> axis = {direccion.get_x(), direccion.get_y(), direccion.get_z()};

    validate_axis_nonzero(axis, lineforprint, "cylinder");

    // PEDIDO EN EL ENUNCIADO
    if (scene.material_index.find(t[7]) == scene.material_index.end()) {
      std::ostringstream oss;
      oss << "Material not found: \"" << "[" << t[7] << "]\"\n"
          << "Line: \"" << lineforprint << "\"";
      throw std::runtime_error(oss.str());
    }
    auto it = scene.material_index.find(t[4]);
    if (it != scene.material_index.end()) {
      // it es un iterador; it->first = clave (string), it->second = valor (size_t)
      std::size_t code    = it->second;
      int n               = static_cast<int>(code);
      int ultimo          = n % 10;  // Valor ficticio para evitar warning de variable no usada
      std::size_t primero = 0;
      while (n >= 10) {
        n /= 10;
        primero = static_cast<std::size_t>(n);
      }

      if (ultimo == 0) {  // SIGNIFICA QUE EL VECTOR DEL MATERIAL ES EL DE MATT
        render::Cylinder c(center, radius, direccion, scene.mattes[primero]);
        scene.add_cylinder(c);
      }
      if (ultimo == 1) {  // SIGNIFICA QUE EL VECTOR DEL MATERIAL ES EL DE METAL
        render::Cylinder c(center, radius, direccion, scene.metales[primero]);
        scene.add_cylinder(c);
      }
      if (ultimo == 2) {  // SIGNIFICA QUE EL VECTOR DEL MATERIAL ES EL DE REFRACTARIO
        render::Cylinder c(center, radius, direccion, scene.refractarios[primero]);
        scene.add_cylinder(c);
      }
    }
  }

  // ============================ parse_scene_stream ============================

  void dispatch_scene_entity(std::string const & tag, std::vector<std::string> const & tokens,
                             Scene & scene, std::string const & lineforprint) {
    if (tag == "matte") {
      parse_matte_line(tokens, scene, lineforprint);
    } else if (tag == "metal") {
      parse_metal_line(tokens, scene, lineforprint);
    } else if (tag == "refractive") {
      parse_refractive_line(tokens, scene, lineforprint);
    } else if (tag == "sphere") {
      parse_sphere_line(tokens, scene, lineforprint);
    } else if (tag == "cylinder") {
      parse_cylinder_line(tokens, scene, lineforprint);
    } else {
      std::ostringstream oss;  // Mensaje pedido por el enunciado
      oss << "Unknown scene entity: " << tag;
      throw std::runtime_error(oss.str());
    }
  }

}  // namespace

// FUNCION IMPORTANTE, ES LA QUE SE LLAMA DESDE FUERA, LE PASAMOS EL STREAM DE ENTRADA Y LA ESCENA
// DONDE VAMOS A GUARDAR LO PARSEADO

namespace parse {

  void parse_scene_stream(std::istream & in, Scene & scene) {
    std::string line;
    std::size_t lineno = 0;
    std::string lineforprint;

    while (std::getline(in, line)) {
      ++lineno;
      lineforprint = line;
      // comentarios
      if (auto p = line.find('#'); p != std::string::npos) {
        line.erase(p);
      }

      line = trim(line);

      if (line.empty()) {
        continue;
      }

      // etiqueta: matte|metal|refractive|sphere|cylinder
      auto colon = line.find(':');
      if (colon == std::string::npos) {
        std::ostringstream oss;
        oss << "Syntax error on line " << lineno << ": missing ':'";
        throw std::runtime_error(oss.str());
      }

      std::string tag  = trim(line.substr(0, colon));
      std::string args = trim(line.substr(colon + 1));
      auto tokens      = split_ws(args);

      dispatch_scene_entity(tag, tokens, scene, lineforprint);
    }
  }

}  // namespace parse
