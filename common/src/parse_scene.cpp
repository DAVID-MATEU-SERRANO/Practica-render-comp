#include "../include/parse_scene.hpp"
#include "../include/scene.hpp"
#include "../include/util.hpp"
#include "color.hpp"
#include "cylinder.hpp"
#include "matte.hpp"
#include "metal.hpp"
#include "parse_exception.hpp"
#include "point.hpp"
#include "refractive.hpp"
#include "sphere.hpp"
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <regex>
#include <stdexcept>

namespace render {

  // Declaro la clase scene aquí por un error que me sale
  class Scene;

  // Estas funciones que van a ser llamadas internamente se guardan en un namespace anónimo
  namespace {

    // Función para obtener material del index
    template <typename T> T get_material_from_code(render::Scene & scene, size_t code) {
      // Obtenemos índice y tipo
      std::size_t const material_index = code / 10;
      int const material_type          = static_cast<int>(code % 10);

      // constexpr permite ejecutar cosas en tiempo de compilación y eliminar ramas innecesarias
      if constexpr (std::is_same_v<T, render::Matte>) {
        if (material_type != 0) {
          throw std::runtime_error("Error obtaining material. Case Mate");
        }
        return scene.get_mattes().at(material_index);
      } else if constexpr (std::is_same_v<T, render::Refractive>) {
        if (material_type != 1) {
          throw std::runtime_error("Error obtaining material. Case refractive");
        }
        return scene.get_refractives().at(material_index);
      } else if constexpr (std::is_same_v<T, render::Metal>) {
        if (material_type != 2) {
          throw std::runtime_error("Error obtaining material. Case Metal");
        }
        return scene.get_metals().at(material_index);
      } else {
        throw std::runtime_error("Obtained not recognized material");
      }
    }

    void parse_matte_line(std::vector<std::string> const & tokens, Scene & scene,
                          std::string const & line_content) {
      parse::util::expect_token_count(tokens, 4, line_content, "matte");

      double r = parse::util::to_double(tokens[1]);
      double g = parse::util::to_double(tokens[2]);
      double b = parse::util::to_double(tokens[3]);

      Color reflectance(r, g, b);
      render::Matte matte(tokens[0], reflectance);
      scene.add_material_matte(matte, line_content);
    }

    void parse_metal_line(std::vector<std::string> const & tokens, Scene & scene,
                          std::string const & line_content) {
      parse::util::expect_token_count(tokens, 5, line_content, "metal");

      double r         = parse::util::to_double(tokens[1]);
      double g         = parse::util::to_double(tokens[2]);
      double b         = parse::util::to_double(tokens[3]);
      double diffusion = parse::util::to_double(tokens[4]);

      Color reflectance(r, g, b);

      if (diffusion < 0.0) {
        parse::throw_invalid_parameters("metal", line_content);
      }

      render::Metal metal(tokens[0], reflectance, diffusion);
      scene.add_material_metal(metal, line_content);
    }

    void parse_refractive_line(std::vector<std::string> const & tokens, Scene & scene,
                               std::string const & line_content) {
      parse::util::expect_token_count(tokens, 2, line_content, "refractive");

      double refraction_index = parse::util::to_double(tokens[1]);

      if (refraction_index < 1.0) {
        parse::throw_invalid_parameters("refractive", line_content);
      }

      render::Refractive refractive(tokens[0], refraction_index);
      scene.add_material_refractive(refractive, line_content);
    }

    void parse_sphere_line(std::vector<std::string> const & tokens, Scene & scene,
                           std::string const & line_content) {
      parse::util::expect_token_count(tokens, 3, line_content, "sphere");
      double cx                         = parse::util::to_double(tokens[0]);
      double cy                         = parse::util::to_double(tokens[1]);
      double cz                         = parse::util::to_double(tokens[2]);
      double radius                     = parse::util::to_double(tokens[3]);
      std::string const & material_name = tokens[4];

      if (radius < 0.0) {
        parse::throw_invalid_parameters("sphere", line_content);
      }

      auto & material_index = scene.get_material_index();
      auto it               = material_index.contains(material_name);
      if (!it) {
        parse::throw_material_not_found(material_name, line_content);
      }

      // Obtener material
      std::size_t code  = material_index.at(material_name);
      int material_type = static_cast<int>(code % 10);  // Está sobre 10 el índice

      render::Point center(cx, cy, cz);

      if (material_type == 0) {
        auto material = get_material_from_code<render::Matte>(scene, code);
        render::Sphere s(center, radius, material);
        scene.add_sphere(s);
      } else if (material_type == 1) {
        auto material = get_material_from_code<render::Metal>(scene, code);
        render::Sphere s(center, radius, material);
        scene.add_sphere(s);
      } else {
        auto material = get_material_from_code<render::Refractive>(scene, code);
        render::Sphere s(center, radius, material);
        scene.add_sphere(s);
      }
    }

    // Usamos una estructura y creamos función por error del clang-tidy
    struct CylinderParams {
      render::Point center;
      render::Vector direction;
      double radius;
      std::string material_name;
    };

    CylinderParams parse_cylinder_geometry(std::vector<std::string> const & t,
                                           std::string const & lineforprint) {
      double cx                         = parse::util::to_double(t[0]);
      double cy                         = parse::util::to_double(t[1]);
      double cz                         = parse::util::to_double(t[2]);
      double radius                     = parse::util::to_double(t[3]);
      double ax                         = parse::util::to_double(t[4]);
      double ay                         = parse::util::to_double(t[5]);
      double az                         = parse::util::to_double(t[6]);
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

      CylinderParams params = parse_cylinder_geometry(tokens, line_content);

      auto & material_index = scene.get_material_index();
      auto it               = material_index.contains(params.material_name);

      if (!it) {
        parse::throw_material_not_found(params.material_name, line_content);
      }

      std::size_t code  = material_index.at(params.material_name);
      int material_type = static_cast<int>(code % 10);

      if (material_type == 0) {
        auto material = get_material_from_code<render::Matte>(scene, code);
        render::Cylinder c(params.center, params.radius, params.direction, material);
        scene.add_cylinder(c);
      } else if (material_type == 1) {
        auto material = get_material_from_code<render::Metal>(scene, code);
        render::Cylinder c(params.center, params.radius, params.direction, material);
        scene.add_cylinder(c);
      } else {
        auto material = get_material_from_code<render::Refractive>(scene, code);
        render::Cylinder c(params.center, params.radius, params.direction, material);
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

  }  // anonymous namespace

}  // namespace render

namespace parse {

  void parse_scene_stream(std::istream & in, render::Scene & scene) {
    std::string line;
    std::size_t lineno = 0;
    // Expresión regular que lleva el tag y sus argumentos
    // \s*: ignora espacios y tabuladores
    // ([a-z]+): captura el tag (solo letras minúsculas)
    // \s*([^:]+?): captura los argumentos (cualquier cosa que no sea ':')
    // \s*(.*): captura cualquier cosa después de los argumentos (espacios o comentarios
    std::regex const scene_line_regex(R"(\s*([a-z]+):\s*([^:]+?)\s*(.*))");
    std::smatch match;

    while (std::getline(in, line)) {
      lineno += lineno;
      std::string const & line_content = line;

      // En caso de que ponga comentarios en los archivos ponemos esto
      /*
        if (auto p = line.find('#'); p != std::string::npos) {
          line.erase(p);
        }
        line = trim(line);
        if (line.empty()) {
          continue;
        }
      */

      // Comprobación
      try {
        if (std::regex_match(line, match, scene_line_regex)) {
          std::string tag   = match[1].str();
          std::string args  = match[2].str();
          std::string extra = match[3].str();  // Usado para comprobar que no se pasa de argumentos

          if (!extra.empty() and util::trim(extra) != "") {
            std::ostringstream oss;
            oss << "Error: Extra data after configuration value for key: " << "[" << tag << "]"
                << "\n"
                << "Extra: \"" << extra << "\"\n"
                << "Line: \"" << line_content << "\"";
            throw std::runtime_error(oss.str());
          }

          // Guardamos argumentos en un vector de strings
          auto tokens = util::split_ws(args);

          render::dispatch_scene_entity(tag, tokens, scene, line_content);
        } else {
          // Etiqueta mal
          std::ostringstream oss;
          oss << "Error: Unknown scene entity: " << line << "\n";
          throw std::runtime_error(oss.str());
        }
      } catch (ParseException const & e) {
        std::cerr << "Error: " << e.what() << "\n"
                  << "Line: \"" << e.get_line_content() << "\n";
        exit(EXIT_FAILURE);
      } catch (std::runtime_error const & e) {
        std::cerr << "Error: " << e.what() << "\n";
        exit(EXIT_FAILURE);
      }
    }
  }

}  // namespace parse
