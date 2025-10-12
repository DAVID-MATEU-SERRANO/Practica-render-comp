#include "../include/cylinder.hpp"

namespace render {

  double Cylinder::get_cords_x() const {
    return cords_x;
  }

  double Cylinder::get_cords_y() const {
    return cords_y;
  }

  double Cylinder::get_cords_z() const {
    return cords_z;
  }

  double Cylinder::get_radius() const {
    return radius;
  }

  double Cylinder::get_height() const {
    return vector.magnitude();
  }

}  // namespace render
