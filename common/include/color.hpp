#ifndef RENDER_COLOR_HPP
#define RENDER_COLOR_HPP

namespace render {

  class Color {
  public:
    Color() : r(0.0), g(0.0), b(0.0) { }

    Color(double r, double g, double b) : r(r), g(g), b(b) { }

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
