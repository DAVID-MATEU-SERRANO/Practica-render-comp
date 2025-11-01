#ifndef RENDER_COLOR_HPP
#define RENDER_COLOR_HPP
#include <cmath>
#include <stdexcept>

namespace render {

  class Color {
  public:
    Color() : r(0.0), g(0.0), b(0.0) {
      if (r < 0.0 or r > 1.0 or g < 0.0 or g > 1.0 or b < 0.0 or b > 1.0) {
        throw std::runtime_error("Error: Invalid color parameters");
      }
    }

    Color(double r, double g, double b) : r(r), g(g), b(b) { }

    // Getters
    [[nodiscard]] double get_r() const { return r; }

    [[nodiscard]] double get_g() const { return g; }

    [[nodiscard]] double get_b() const { return b; }

    [[nodiscard]] Color multiply(double factor) const {
      return {r * factor, g * factor, b * factor};
    }

    // Operators
    [[nodiscard]] Color multiply(Color const & other) const {
      return {r * other.r, g * other.g, b * other.b};
    }

    [[nodiscard]] Color add(Color const & other) const {
      return {r + other.r, g + other.g, b + other.b};
    }

    [[nodiscard]] Color apply_gamma_correction(double gamma) const {
      return {std::pow(r, 1.0 / gamma), std::pow(g, 1.0 / gamma), std::pow(b, 1.0 / gamma)};
    }

  private:
    double r;
    double g;
    double b;
  };

}  // namespace render

#endif
