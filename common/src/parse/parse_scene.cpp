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

  [[noreturn]] inline void parse_error(size_t line, std::string const & msg) {
    std::ostringstream oss;
    oss << "Error de parseo en linea " << line << ": " << msg;
    throw std::runtime_error(oss.str());
  }

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
    }
  }

}  // namespace
