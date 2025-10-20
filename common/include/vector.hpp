#ifndef RENDER_VECTOR_HPP
#define RENDER_VECTOR_HPP

#include <cmath>

namespace render {

  class Vector {
  public:
    Vector() : x{0.0}, y{0.0}, z{0.0} { }

    Vector(double cx, double cy, double cz) : x{cx}, y{cy}, z{cz} { }

    [[nodiscard]] double magnitude() const;
    [[nodiscard]] Vector normalized() const;
    [[nodiscard]] Vector add(Vector const & other) const;
    [[nodiscard]] Vector substract(Vector const & other) const;
    [[nodiscard]] Vector dot(double scalar) const;
    [[nodiscard]] double dot(Vector const & other) const;
    [[nodiscard]] Vector cross(Vector const & other) const;
    [[nodiscard]] Vector perpendicular_component(Vector const & other) const;

    [[nodiscard]] double get_x() const;
    [[nodiscard]] double get_y() const;
    [[nodiscard]] double get_z() const;

  private:
    double x, y, z;
  };

}  // namespace render

#endif
