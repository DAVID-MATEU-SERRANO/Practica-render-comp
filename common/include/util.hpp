#include <array>
#include <cctype>
#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

// ================================ Helpers ================================
namespace parse::util {

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
  // sobrecarga de la llamada a función y mejorar el rendimiento en funciones pequeñas, porque los
  // helpers estos los voy a usar mucho

  /*[[noreturn]] inline void parse_error(
      std::size_t line, std::string const & msg) {

    std::ostringstream oss;
    oss << "Parsing error in line " << line << ": "
        << msg;
    throw std::runtime_error(
        oss.str());  /

  } */

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
  // Me hice fan de los istringstream y ostringstream

  inline double to_double(
      std::string const & s, std::string const & lineforprint,
      char const * where) {  // Convierte una cadena a double, lanza error si no se puede convertir
    try {
      return std::stod(s);  // std::stod convierte string a double
    } catch (...) {         // Si hay cualquier error (no se puede convertir)
      std::ostringstream oss;
      oss << "Invalid" << " " << where << " parameters" << "\n"
          << "Line: \"" << lineforprint << "\"";
      throw std::runtime_error(oss.str());  // Lanzamos el error con el mensaje
    }
  }

  inline double to_double_config(
      std::string const & s, std::string const & lineforprint,
      char const * where) {  // Convierte una cadena a double, lanza error si no se puede convertir
    try {
      return std::stod(s);  // std::stod convierte string a double
    } catch (...) {         // Si hay cualquier error (no se puede convertir)
      std::ostringstream oss;
      oss << "Invalid" << " " << where << " parameters" << "\n"
          << "Line: \"" << lineforprint << "\"";
      throw std::runtime_error(oss.str());  // Lanzamos el error con el mensaje
    }
  }

  // Simple, no creo que se necesite mucha explicación, intenta convertir la cadena s a double
  // usando std::stod

  inline void expect_token_count(
      std::vector<std::string> const & t,
      std::size_t n,  // Verifica que el número de tokens sea el esperado
      std::string const & lineforprint,
      std::string const &
          label) {  // t es el vector de tokens, n es el número esperado, lineno es la línea actual

    std::string attr;
    if (label == "matte" or label == "metal" or label == "refractive") {
      attr = "material";
    } else {
      attr = "object";
    }
    //=============ERRORES PEDIDOS EN LA PRACTICA===============
    if (t.size() < n) {
      std::ostringstream oss;
      oss << "Invalid " << label << " " << attr << " parameters\n"
          << "Line: \"" << lineforprint << "\"";
      throw std::runtime_error(oss.str());  // Lanzamos el error con el mensaje
    }
    if (t.size() > n) {
      std::ostringstream oss;
      oss << "Extra data after configuration value for key:" << " " << "[" + label + "]" << "\n"
          << "Extra:" << " " << +(t.size() - n) << "\n"
          << "Line: \"" << lineforprint << "\"";
      throw std::runtime_error(oss.str());  // Lanzamos el error con el mensaje
    }
  }

  // Simple y al final acabamos usando parse_error para lanzar el error si no coincide el número de
  // tokens, PERO QUE EFICIENTES SOMOS

  inline void validate_rgb(std::array<double, 3> const & color, std::string const & lineforprint,
                           std::string const & where) {
    auto in01 = [](double x) { return x >= 0.0 and x <= 1.0; };
    if (!in01(color[0]) or !in01(color[1]) or !in01(color[2])) {
      std::ostringstream oss;
      oss << "Invalid" << " " << where << " parameters" << "\n"
          << "Line: \"" << lineforprint << "\"";

      throw std::runtime_error(oss.str());
    }
  }

  inline void validate_rgb_config(std::array<double, 3> const & colors,
                                  std::string const & lineforprint, std::string_view const & key) {
    auto in01 = [](double x) { return x >= 0.0 and x <= 1.0; };
    if (!in01(colors[0]) or !in01(colors[1]) or !in01(colors[2])) {
      std::ostringstream oss;
      oss << "Invalid value for key: < " << key << ">\n"
          << "Line: \"" << lineforprint << "\"";

      throw std::runtime_error(oss.str());
    }
  }

  // OJO ESTO ESTA INTERESANTE, CREAMOS UNA LAMBDA (FUNCION ANONIMA) PARA VERIFICAR SI UN VALOR ESTA
  // ENTRE 0 Y 1, basicamente es como una función pequeña que solo se usa aquí, y la usamos para r,
  // g y b ES UNA FUNCION DENTRO DE OTRA, me lo sugirio ya sabemos quien y me gusto Si alguno no
  // está en el rango, lanzamos un error

  inline void validate_axis_nonzero(std::array<double, 3> const & axis,
                                    std::string const & lineforprint, std::string const & where) {
    if (axis[0] == 0.0 and axis[1] == 0.0 and axis[2] == 0.0) {
      std::ostringstream oss;
      oss << "Invalid" << " " << where << " parameters" << "\n"
          << "Line: \"" << lineforprint << "\"";
      throw std::runtime_error(oss.str());
    }
  }

  // BUENO esta es muy simple, verifica que el cilindro no tenga un eje nulo (0,0,0), porque eso no
  // tiene sentido para un cilindro

  inline int to_int(std::string const & s, std::string const & lineforprint, char const * what) {
    try {
      return std::stoi(s);  // std::stoi convierte string a int
    } catch (...) {         // Si hay cualquier error (no se puede convertir)
      std::ostringstream oss;
      oss << "Invalid value for key:" << " " << "[" << what << "]\n"
          << "Line: \"" << lineforprint << "\"";
      throw std::runtime_error(oss.str());
    }
  }

  inline void parse_three_doubles(std::string const & val, std::array<double, 3> & out,
                                  std::string const & lineforprint, char const * what) {
    std::istringstream iss(val);
    if (!(iss >> out[0] >> out[1] >> out[2])) {
      std::ostringstream oss;
      oss << "Invalid" << " " << what << " parameters" << "\n"
          << "Line: \"" << lineforprint << "\"";
      throw std::runtime_error(oss.str());
    }
  }

  inline void expect_positive(int v, std::string const & lineforprint, char const * what) {
    if (v <= 0) {
      std::ostringstream oss;
      oss << what << " must be positive (integer), got: " << v << "\n"
          << "Line: \"" << lineforprint << "\"";
      throw std::runtime_error(oss.str());
    }
  }

  inline std::string strip_comment_and_trim(std::string line) {
    if (auto pos = line.find('#'); pos != std::string::npos) {
      line = line.erase(pos);
    }
    return trim(line);
  }

  inline uint64_t to_uint64(std::string const & s, std::string const & lineforprint,
                            char const * what) {
    try {
      return static_cast<uint64_t>(
          std::stoull(s));  // std::stoull convierte string a unsigned long long
    } catch (...) {         // Si hay cualquier error (no se puede convertir)
      std::ostringstream oss;
      oss << "Cannot be converted to uint64 (" << what << "): \"" << s << "\"\n"
          << "Line: \"" << lineforprint << "\"";
      throw std::runtime_error(oss.str());
    }
  }

}  // namespace parse::util
