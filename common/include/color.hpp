#ifndef RENDER_COLOR_HPP
#define RENDER_COLOR_HPP

#include <stdexcept>

namespace render {

  class Color {
  public:
    Color(double r, double g, double b) : r(r), g(g), b(b) {
      if (r < 0.0 or r > 1.0 or g < 0.0 or g > 1.0 or b < 0.0 or b > 1.0) {
        throw std::out_of_range("Error rgb values less than 0 or more than 1\n");
      }
    }

    [[nodiscard]] double get_r() const;
    [[nodiscard]] double get_g() const;
    [[nodiscard]] double get_b() const;
    [[nodiscard]] Color multiply(double factor) const;
    [[nodiscard]] Color multiply(Color const & other) const;
    [[nodiscard]] Color add(Color const & other) const;
    [[nodiscard]] Color apply_gamma_correction(double gamma) const;

  private:
    double r;
    double g;
    double b;
  };

}  // namespace render

#endif
