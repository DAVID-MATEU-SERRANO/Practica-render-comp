#include "../include/point.hpp"
#include "vector.hpp"

namespace render {

  double Point::get_x() const {
    return x;
  }

  double Point::get_y() const {
    return y;
  }

  double Point::get_z() const {
    return z;
  }

  Vector Point::substract(Point const & other) const {
    return Vector{x - other.get_x(), y - other.get_y(), z - other.get_z()};
  }

  Point Point::substract(Vector const & other) const {
    return Point{x - other.get_x(), y - other.get_y(), z - other.get_z()};
  }

}  // namespace render
