#include "cylinder.hpp"

namespace render {

  Point Cylinder::get_center() const {
    return vec_center;
  }

  Vector Cylinder::get_edge() const {
    return vec_edge.normalized();
  }

  double Cylinder::get_radius() const {
    return radius;
  }

  double Cylinder::get_height() const {
    return vec_edge.magnitude();
  }

}  // namespace render
