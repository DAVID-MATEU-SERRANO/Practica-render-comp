#include "sphere.hpp"

namespace render {

  double Sphere::get_radius() const {
    return radius;
  }

  Point Sphere::get_center() const {
    return sphere_center;
  }

}  // namespace render
