#include "../../include/parse/parse_scene.hpp"
#include "../../include/parse/scene.hpp"

#include <cctype>
#include <cstddef>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

// ================================ Helpers ================================
namespace {

  inline std::string trim(
      std::string const & s) {   // Función para eliminar los saltos de linea y espacios en blanco
    size_t i = 0, j = s.size();  // i apunta al primer caracter no blanco, j al último+1
    while (i < j and std::isspace(static_cast<unsigned char>(s[i])) != 0)
    {       // Primer while avanza desde el inicio hasta encontrar un caracter no blanco
      ++i;  // la biblioteca de isspace es cctype y funciona super raro
    }
    while (j > i and std::isspace(static_cast<unsigned char>(s[j - 1])) != 0)
    {  // Segundo while retrocede desde el final para encontrar un caracter no blanco
      --j;
    }
    return s.substr(i, j - i);  // Devuelve el substring entre i y j-1
  }

  // EXPLICACION: BASICAMENTE SI EN EL PARSEO SE LEIA "     HOLA     "  --> TE DEVUELVE "HOLA"

  // Mas aclaraciones por si acaso es size_t y no int, es porque size_t es un entero sin signo
  // (unsigned) que se usa para tamaños y conteos, conviene usar size_t

  // Ah y el inline es para que el compilador lo ponga en el lugar donde se llama, para evitar la
  // sobrecarga de la llamada a función y mejorar el rendimiento en funciones pequeñas, por que los
  // helpers estos los voy a usar mucho

  [[noreturn]] inline void parse_error(
      std::size_t line, std::string const & msg) {  // Función para lanzar errores de parseo,
                                                    // noreturn para que no vuelva a donde la llame
    std::ostringstream oss;  // Obviameente no vuelve si hemos lanzado un error
    oss << "Parsing error in line " << line << ": "
        << msg;  // line es el número de línea y msg es el mensaje de error
    throw std::runtime_error(
        oss.str());  // LO INTERESANTE: el ostringstream es para construir cadenas de texto de forma
                     // eficiente, parecido a cout pero para strings
  }  // basicamente es como hacer un cout pero que se almacena en la variable oss

  // Que por que use el ostringstream y no un string normal? Pues porque he leido que es más
  // eficiente para concatenar múltiples partes de texto, especialmente en bucles o cuando se
  // construyen mensajes complejos. Luego lanzamos el error con lo almacenado en oss.str() que
  // convierte el ostringstream a un string normal

  inline std::vector<std::string> split_ws(
      std::string const & s) {  // Función para separar una cadena en tokens usando espacios en
                                // blanco como separadores
    std::istringstream iss(s);  // istringstream es como ostringstream pero para leer strings como
                                // si fueran flujos de entrada
    std::vector<std::string> out;  // Vector para almacenar los tokens
    std::string tok;               // Variable temporal para cada token
    while (iss >> tok) {           // Lee tokens separados por espacios en blanco
      out.push_back(tok);          // Añade el token al vector
    }
    return out;
  }

  // Basicamente lo que estamos haciendo es leer la cadena s como si fuera un flujo de entrada, y
  // cada vez que encontramos un token (una palabra separada por espacios) lo añadimos al vector out
  // Al final devolvemos el vector con todos los tokens encontrados
  // Me hice fan de los istringstream y ostringstream, seguro el profe me dice que no los use

  inline double to_double(
      std::string const & s, std::size_t lineno,
      char const * what) {  // Convierte una cadena a double, lanza error si no se puede convertir
    try {
      return std::stod(s);  // std::stod convierte string a double
    } catch (...) {         // Si hay cualquier error (no se puede convertir)
      parse_error(lineno,
                  std::string("Cannot be converted to number (") + what + "): \"" + s + "\"");
    }
  }

  // Simple, no creo que se necesite mucha explicación, intenta convertir la cadena s a double
  // usando std::stod

  inline void expect_token_count(
      std::vector<std::string> const & t,
      std::size_t n,  // Verifica que el número de tokens sea el esperado
      std::size_t lineno,
      std::string const &
          label) {  // t es el vector de tokens, n es el número esperado, lineno es la línea actual
                    // para errores, label es la etiqueta (matte, metal, etc)
    if (t.size() != n) {       // Si el tamaño del vector no es el esperado
      std::ostringstream oss;  // Usamos ostringstream para construir el mensaje de error
      oss << label << " waiting " << n << " arguments, got " << t.size();
      parse_error(lineno, oss.str());
    }
  }

  // Simple y al final acabamos usando parse_error para lanzar el error si no coincide el número de
  // tokens, PERO QUE EFICIENTES SOMOS

  inline void validate_rgb(double r, double g, double b, std::size_t lineno) {
    auto in01 = [](double x) {
      return x >= 0.0 and x <= 1.0;
    };  // Lambda para verificar si un valor está en [0,1
    if (!in01(r) or !in01(g) or !in01(b)) {
      parse_error(lineno, "Color out of range [0,1]");
    }
  }

  // OJO ESTO ESTA INTERESANTE, CREAMOS UNA LAMBDA (FUNCION ANONIMA) PARA VERIFICAR SI UN VALOR ESTA
  // ENTRE 0 Y 1, basicamente es como una función pequeña que solo se usa aquí, y la usamos para r,
  // g y b ES UNA FUNCION DENTRO DE OTRA, me lo sugirio ya sabemos quien y me gusto Si alguno no
  // está en el rango, lanzamos un error

  inline void validate_axis_nonzero(double x, double y, double z, std::size_t lineno) {
    if (x == 0.0 and y == 0.0 and z == 0.0) {
      parse_error(lineno, "Cylinder's edge cannot be (0,0,0)");
    }
  }

  // BUENO esta es muy simple, verifica que el cilindro no tenga un eje nulo (0,0,0), porque eso no
  // tiene sentido para un cilindro

  // ============================ Parsers por etiqueta ============================

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
