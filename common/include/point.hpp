#ifndef RENDER_POINT_HPP
#define RENDER_POINT_HPP

#include "vector.hpp"
#include <cmath>

namespace render {

  class Point {
  public:
    Point(double cx, double cy, double cz) : x{cx}, y{cy}, z{cz} { }

    [[nodiscard]] double get_x() const;
    [[nodiscard]] double get_y() const;
    [[nodiscard]] double get_z() const;
    [[nodiscard]] Vector substract(Point const & other) const;
    [[nodiscard]] Point substract(Vector const & other) const;

  private:
    double x, y, z;
  };

}  // namespace render

#endif
