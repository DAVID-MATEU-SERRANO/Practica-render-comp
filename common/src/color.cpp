#include "../include/color.hpp"
#include <cmath>
#include <stdexcept>

namespace render {

  double Color::get_r() const {
    if (r < 0 or r > 1) {
      throw std::runtime_error("Error: Invalid color parameters");
    }
    return r;
  }

  double Color::get_g() const {
    if (g < 0 or g > 1) {
      throw std::runtime_error("Error: Invalid color parameters");
    }
    return g;
  }

  double Color::get_b() const {
    if (b < 0 or b > 1) {
      throw std::runtime_error("Error: Invalid color parameters");
    }
    return b;
  }

  Color Color::multiply(double factor) const {
    return {r * factor, g * factor, b * factor};
  }

  Color Color::multiply(Color const & other) const {
    return {r * other.r, g * other.g, b * other.b};
  }

  Color Color::add(Color const & other) const {
    return {r + other.r, g + other.g, b + other.b};
  }

  Color Color::apply_gamma_correction(double gamma) const {
    return {std::pow(r, 1.0 / gamma), std::pow(g, 1.0 / gamma), std::pow(b, 1.0 / gamma)};
  }

}  // namespace render
