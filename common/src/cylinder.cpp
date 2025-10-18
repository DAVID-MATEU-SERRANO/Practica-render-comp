#include "../include/cylinder.hpp"

namespace render {

  double Cylinder::get_cords_x() const {
    return vec_center.get_x();
  }

  double Cylinder::get_cords_y() const {
    return vec_center.get_y();
  }

  double Cylinder::get_cords_z() const {
    return vec_center.get_z();
  }

  double Cylinder::get_radius() const {
    return radius;
  }

  double Cylinder::get_height() const {
    return vec_edge.magnitude();
  }

}  // namespace render
