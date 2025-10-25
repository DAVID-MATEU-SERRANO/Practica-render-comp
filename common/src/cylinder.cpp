#include "../include/cylinder.hpp"
#include "../include/point.hpp"
#include "../include/vector.hpp"
#include <iostream>
#include <stdexcept>

namespace render {

  Point Cylinder::get_center() const {
    if (vec_center.get_x() < 0.0 or vec_center.get_y() < 0.0 or vec_center.get_z() < 0.0) {
      throw std::runtime_error("Error: Invalid cylinder parameters");
    }
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

}  // namespace render
