#include "../include/cylinder.hpp"
#include "../include/point.hpp"
#include "../include/vector.hpp"
#include <stdexcept>

namespace render {

  Point Cylinder::get_center() const {
    return vec_center;
  }

  Vector Cylinder::get_edge() const {
    if (vec_edge.magnitude() == 0.0) {
      throw std::runtime_error("Error: Invalid cylinder parameters");
    }
    return vec_edge.normalized();
  }

  double Cylinder::get_radius() const {
    if (radius < 0.0) {
      throw std::runtime_error("Error: Invalid cylinder parameters");
    }
    return radius;
  }

  double Cylinder::get_height() const {
    return vec_edge.magnitude();
  }

  t_material Cylinder::get_material() const {
    return material;
  }

}  // namespace render
