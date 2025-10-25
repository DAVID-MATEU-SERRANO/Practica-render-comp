#include "../include/sphere.hpp"
#include "../include/point.hpp"
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

  t_material Sphere::get_material() const {
    return material;
  }

}  // namespace render
