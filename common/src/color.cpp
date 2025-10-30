#include "../include/color.hpp"
#include <cmath>

namespace render {

  double Color::get_r() const {
    return r;
  }

  double Color::get_g() const {
    return g;
  }

  double Color::get_b() const {
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
