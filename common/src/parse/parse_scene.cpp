#include "../../include/parse/parse_scene.hpp"
#include "../../include/parse/scene.hpp"

#include <cctype>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

// =================================Helpers horribles====================================

namespace {

  // Quitar espacios al inicio y final
  inline std::string trim(std::string const & s) {
    size_t i = 0, j = s.size();
    while (i < j and std::isspace(static_cast<unsigned char>(s[i])) != 0) {
      ++i;
    }
    while (j > i and std::isspace(static_cast<unsigned char>(s[j - 1])) != 0) {
      --j;
    }
    return s.substr(i, j - i);
  }

  // Manejo de errores
  [[noreturn]] inline void parse_error(size_t line, std::string const & msg) {
    std::ostringstream oss;
    oss << "Error de parseo en linea " << line << ": " << msg;
    throw std::runtime_error(oss.str());
  }

  // Separar linea por espacios en blanco
  inline std::vector<std::string> split_ws(std::string const & s) {
    std::istringstream iss(s);
    std::vector<std::string> out;
    std::string tok;
    while (iss >> tok) {
      out.push_back(tok);
    }
    return out;
  }

}  // namespace

//===========================LOGICA DEL PARSER (LETS GO MODO SALVAJE)===========================

void parse_scene_stream(std::istream & in, Scene & scene) {
  enum class State { Top, InMaterial };
  State st = State::Top;

  std::string line;
  size_t lineno = 0;

  // buffers para datos de materiales

  std::string name;
  float r = 1.F, g = 1.F, b = 1.F;
  float rough = 0.5F;

  auto reset_material = [&]() {
    name.clear();
    r     = 1.F;
    g     = 1.F;
    b     = 1.F;
    rough = 0.5F;
  };

  // LEER LINEA POR LINEA (MATEU SI LEES ESTO ERES UN GENIO)

  while (std::getline(in, line)) {
    ++lineno;

    // eliminar comentarios
    if (auto p = line.find('#'); p != std::string::npos) {
      line.erase(p);
    }
    line = trim(line);
    if (line.empty()) {
      continue;
    }

    // Fuera de bloque

    if (st == State::Top) {
      if (line == "material") {
        st = State::InMaterial;

        reset_material();
        continue;
      }
      parse_error(lineno, "Comando inesperado fuera de bloque" + line + "");
    }

    // Dentro de bloque material
    else
    {
      if (line == "}") {
        if (name.empty()) {
          parse_error(lineno, "Material sin nombre");
        }

        Material m;
        m.name      = name;
        m.color     = {r, g, b};
        m.roughness = rough;

        // Guardar material en la escena
        scene.add_material(m);

        st = State::Top;
        continue;
      }

      // Esperamos key value
      auto eq = line.find('=');
      if (eq == std::string::npos) {
        parse_error(lineno, "Esperando key=value, got: " + line + "");

        std::string key = trim(line.substr(0, eq));
        std::string val = trim(line.substr(eq + 1));

        if (key == "name") {
          if (val.empty()) {
            parse_error(lineno, "Nombre de material vacio");
          }
          name = val;
        } else if (key == "color") {
          auto t = split_ws(val);
          if (t.size() != 3) {
            parse_error(lineno, "Esperando 3 valores para color, got: " + val + "");
          }
          try {
            r = std::stof(t[0]);
            g = std::stof(t[1]);
            b = std::stof(t[2]);
          } catch (...) {
            parse_error(lineno, "Error al convertir color a float: " + val + "");
          }
        } else if (key == "roughness") {
          try {
            rough = std::stof(val);
          } catch (...) {
            parse_error(lineno, "Error al convertir roughness a float: " + val + "");
          }
          if (rough < 0.F or rough > 1.F) {
            parse_error(lineno, "Roughness fuera de rango [0,1]: " + val + "");
          }
        } else {
          parse_error(lineno, "Key desconocida en material: " + key + "");
        }
      }
    }
  }
}
