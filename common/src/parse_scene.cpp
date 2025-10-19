#include "parse_scene.hpp"
#include "scene.hpp"
#include "util.hpp"

#include <cctype>
#include <cstddef>
#include <istream>
#include <ostream>
#include <string>
#include <vector>

using namespace parse::util;

// ============================ Parsers por etiqueta ============================

namespace {

  void parse_matte_line(std::vector<std::string> const & t, Scene & scene,
                        std::string const & lineforprint) {
    // matte: <name> r g b
    expect_token_count(t, 4, lineforprint, "matte");
    Material m;
    m.type   = "matte";
    m.name   = t[0];
    double r = to_double(t[1], lineforprint, "matte");
    double g = to_double(t[2], lineforprint, "matte");
    double b = to_double(t[3], lineforprint, "matte");

    std::array<double, 3> color = {r, g, b};

    validate_rgb(color, lineforprint, "matte");
    m.color = color;
    scene.add_material(m, lineforprint);
  }

  void parse_metal_line(std::vector<std::string> const & t, Scene & scene,
                        std::string const & lineforprint) {
    // metal: <name> r g b roughness
    expect_token_count(t, 5, lineforprint, "metal");
    Material m;
    m.type                      = "metal";
    m.name                      = t[0];
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
    m.color     = {r, g, b};
    m.roughness = rough;
    scene.add_material(m, lineforprint);
  }

  void parse_refractive_line(std::vector<std::string> const & t, Scene & scene,
                             std::string const & lineforprint) {
    // refractive: <name> ior
    expect_token_count(t, 2, lineforprint, "refractive");
    Material m;
    m.type     = "refractive";
    m.name     = t[0];
    double ior = to_double(t[1], "ior", "refractive");
    if (ior <= 1.0) {
      std::ostringstream oss;
      oss << "Invalid" << " " << "refractive" << " parameters" << "\n"
          << "Line: \"" << lineforprint << "\"";
      throw std::runtime_error(oss.str());
    }
    m.refractive_index = ior;
    scene.add_material(m, lineforprint);
  }

  void parse_sphere_line(std::vector<std::string> const & t, Scene & scene,
                         std::string const & lineforprint) {
    // sphere: cx cy cz radius material_name
    expect_token_count(t, 5, lineforprint, "sphere");
    Sphere s;
    s.type   = "sphere";
    s.center = {to_double(t[0], lineforprint, "sphere"), to_double(t[1], lineforprint, "sphere"),
                to_double(t[2], lineforprint, "sphere")};
    s.radius = to_double(t[3], lineforprint, "sphere");
    if (s.radius <= 0.0) {
      std::ostringstream oss;
      oss << "Invalid" << " " << "sphere" << " parameters" << "\n"
          << "Line: \"" << lineforprint << "\"";
      throw std::runtime_error(oss.str());
    }
    s.material_name = t[4];

    // Verificar que el material exista PEDIDO EN EL ENUNCIADO
    if (scene.material_index.find(s.material_name) == scene.material_index.end()) {
      std::ostringstream oss;
      oss << "Material not found: \"" << "[" << s.material_name << "]\"\n"
          << "Line: \"" << lineforprint << "\"";
      throw std::runtime_error(oss.str());
    }
    scene.add_sphere(s);
  }

  void parse_cylinder_line(std::vector<std::string> const & t, Scene & scene,
                           std::string const & lineforprint) {
    // cylinder: bx by bz ax ay az radius material_name
    expect_token_count(t, 8, lineforprint, "cylinder");
    Cylinder c;
    c.type = "cylinder";
    c.base = {to_double(t[0], lineforprint, "cylinder"), to_double(t[1], lineforprint, "cylinder"),
              to_double(t[2], lineforprint, "cylinder")};
    c.axis = {to_double(t[3], lineforprint, "cylinder"), to_double(t[4], lineforprint, "cylinder"),
              to_double(t[5], lineforprint, "cylinder")};
    c.radius = to_double(t[6], lineforprint, "cylinder");
    if (c.radius <= 0.0) {
      std::ostringstream oss;
      oss << "Invalid" << " " << "cylinder" << " parameters" << "\n"
          << "Line: \"" << lineforprint << "\"";
      throw std::runtime_error(oss.str());
    }

    validate_axis_nonzero(c.axis, lineforprint, "cylinder");
    c.material_name = t[7];

    // PEDIDO EN EL ENUNCIADO
    if (scene.material_index.find(c.material_name) == scene.material_index.end()) {
      std::ostringstream oss;
      oss << "Material not found: \"" << "[" << c.material_name << "]\"\n"
          << "Line: \"" << lineforprint << "\"";
      throw std::runtime_error(oss.str());
    }
    scene.add_cylinder(c);
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
