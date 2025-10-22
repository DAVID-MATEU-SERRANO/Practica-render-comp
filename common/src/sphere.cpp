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
    return sphere_center;
  }

}  // namespace render
