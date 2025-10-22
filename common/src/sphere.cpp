#include "sphere.hpp"
#include <stdexcept>

namespace render {

  double Sphere::get_radius() const {
    if (radius < 0.0) {
      throw std::runtime_error("Error: Invalid sphere parameters");
    }
    return radius;
  }

  Point Sphere::get_center() const {
    if (sphere_center.get_x() < 0.0 or sphere_center.get_y() < 0.0 or sphere_center.get_z() < 0.0) {
      throw std::runtime_error("Error: Invalid sphere parameters");
    }
    return sphere_center;
  }

}  // namespace render
