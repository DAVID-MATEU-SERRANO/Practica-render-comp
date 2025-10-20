#include "parse_scene.hpp"
#include "scene.hpp"
#include "util.hpp"

#include <cctype>
#include <cstddef>
#include <istream>
#include <string>
#include <vector>

using namespace parse::util;

// ============================ Parsers por etiqueta ============================

namespace {

  void parse_matte_line(std::vector<std::string> const & t, Scene & scene, std::size_t lineno) {
    // matte: <name> r g b
    expect_token_count(t, 4, lineno, "matte");
    Material m;
    m.type   = "matte";
    m.name   = t[0];
    double r = to_double(t[1], lineno, "r");
    double g = to_double(t[2], lineno, "g");
    double b = to_double(t[3], lineno, "b");
    validate_rgb(r, g, b, lineno);
    m.color = {r, g, b};
    scene.add_material(m);
  }

  void parse_metal_line(std::vector<std::string> const & t, Scene & scene, std::size_t lineno) {
    // metal: <name> r g b roughness
    expect_token_count(t, 5, lineno, "metal");
    Material m;
    m.type       = "metal";
    m.name       = t[0];
    double r     = to_double(t[1], lineno, "r");
    double g     = to_double(t[2], lineno, "g");
    double b     = to_double(t[3], lineno, "b");
    double rough = to_double(t[4], lineno, "roughness");
    validate_rgb(r, g, b, lineno);
    if (rough < 0.0 or rough > 1.0) {
      parse_error(lineno, "Roughness out of range [0,1]");
    }
    m.color     = {r, g, b};
    m.roughness = rough;
    scene.add_material(m);
  }

  void parse_refractive_line(std::vector<std::string> const & t, Scene & scene,
                             std::size_t lineno) {
    // refractive: <name> ior
    expect_token_count(t, 2, lineno, "refractive");
    Material m;
    m.type     = "refractive";
    m.name     = t[0];
    double ior = to_double(t[1], lineno, "ior");
    if (ior <= 1.0) {
      parse_error(lineno, "Refraction index (ior) must be > 1.0");
    }
    m.refractive_index = ior;
    scene.add_material(m);
  }

  void parse_sphere_line(std::vector<std::string> const & t, Scene & scene, std::size_t lineno) {
    // sphere: cx cy cz radius material_name
    expect_token_count(t, 5, lineno, "sphere");
    Sphere s;
    s.type   = "sphere";
    s.center = {to_double(t[0], lineno, "cx"), to_double(t[1], lineno, "cy"),
                to_double(t[2], lineno, "cz")};
    s.radius = to_double(t[3], lineno, "radius");
    if (s.radius <= 0.0) {
      parse_error(lineno, "Sphere radius must be > 0");
    }
    s.material_name = t[4];

    // Verificar que el material exista
    if (scene.material_index.find(s.material_name) == scene.material_index.end()) {
      parse_error(lineno, "Material not found: \"" + s.material_name + "\"");
    }
    scene.add_sphere(s);
  }

  void parse_cylinder_line(std::vector<std::string> const & t, Scene & scene, std::size_t lineno) {
    // cylinder: bx by bz ax ay az radius material_name
    expect_token_count(t, 8, lineno, "cylinder");
    Cylinder c;
    c.type   = "cylinder";
    c.base   = {to_double(t[0], lineno, "bx"), to_double(t[1], lineno, "by"),
                to_double(t[2], lineno, "bz")};
    c.axis   = {to_double(t[3], lineno, "ax"), to_double(t[4], lineno, "ay"),
                to_double(t[5], lineno, "az")};
    c.radius = to_double(t[6], lineno, "radius");
    if (c.radius <= 0.0) {
      parse_error(lineno, "Radio de cilindro debe ser > 0");
    }
    validate_axis_nonzero(c.axis[0], c.axis[1], c.axis[2], lineno);
    c.material_name = t[7];

    if (scene.material_index.find(c.material_name) == scene.material_index.end()) {
      parse_error(lineno, "Material not found: \"" + c.material_name + "\"");
    }
    scene.add_cylinder(c);
  }

  // ============================ parse_scene_stream ============================

  void dispatch_scene_entity(std::string const & tag, std::vector<std::string> const & tokens,
                             Scene & scene, std::size_t lineno) {
    if (tag == "matte") {
      parse_matte_line(tokens, scene, lineno);
    } else if (tag == "metal") {
      parse_metal_line(tokens, scene, lineno);
    } else if (tag == "refractive") {
      parse_refractive_line(tokens, scene, lineno);
    } else if (tag == "sphere") {
      parse_sphere_line(tokens, scene, lineno);
    } else if (tag == "cylinder") {
      parse_cylinder_line(tokens, scene, lineno);
    } else {
      parse_error(lineno, "Unknown scene entity: " + tag);
    }
  }

}  // namespace

// FUNCION IMPORTANTE, ES LA QUE SE LLAMA DESDE FUERA, LE PASAMOS EL STREAM DE ENTRADA Y LA ESCENA
// DONDE VAMOS A GUARDAR LO PARSEADO

namespace parse {

  void parse_scene_stream(std::istream & in, Scene & scene) {
    std::string line;
    std::size_t lineno = 0;

    while (std::getline(in, line)) {
      ++lineno;

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
        parse_error(lineno, "Espected tag with ':', got: \"" + line + "\"");
      }

      std::string tag  = trim(line.substr(0, colon));
      std::string args = trim(line.substr(colon + 1));
      auto tokens      = split_ws(args);

      dispatch_scene_entity(tag, tokens, scene, lineno);
    }
  }

}  // namespace parse
