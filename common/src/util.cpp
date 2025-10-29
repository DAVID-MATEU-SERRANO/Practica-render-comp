#include "../include/util.hpp"
#include "../include/parse_exception.hpp"
#include <exception>
#include <stdexcept>

namespace parse::util {

  double to_double(std::string const & token) {
    try {
      size_t pos   = 0;
      double value = std::stod(token, &pos);
      return value;

    } catch (std::exception const & e) {
      throw std::runtime_error("Conversion error: not a valid double");
    }
  }

  void expect_token_count(
      std::vector<std::string> const & tokens, size_t expected_count,
      std::string const & line_content,
      std::string const & entity_type) {  // t es el vector de tokens, n es el número esperado,
                                          // lineno es la línea actual

    if (tokens.size() != expected_count) {
      throw_invalid_parameters(entity_type, line_content);
    }
  }

  void validate_rgb_config(std::array<double, 3> const & color, std::string const & line_content,
                           std::string const & entity_type) {
    for (double component : color) {
      if (component < 0.0 or component > 1.0) {
        throw_invalid_parameters(entity_type, line_content);
      }
    }
  }

}  // namespace parse::util
