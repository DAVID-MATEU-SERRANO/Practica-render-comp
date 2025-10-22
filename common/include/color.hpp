#ifndef RENDER_COLOR_HPP
#define RENDER_COLOR_HPP

namespace render {

  class Color {
  public:
    Color(int r, int g, int b) : r(r), g(g), b(b) { }

    [[nodiscard]] int get_r() const;
    [[nodiscard]] int get_g() const;
    [[nodiscard]] int get_b() const;

  private:
    int r;
    int g;
    int b;
  };

}  // namespace render

#endif
