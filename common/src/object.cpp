#include "object.hpp"

namespace render {

  object::object(std::string type, double cords_x, double cords_y, double cords_z, double radius)
      : type_(std::move(type)), cords_x(cords_x), cords_y(cords_y), cords_z(cords_z),
        radius(radius) { }

  std::string object::get_type() const {
    return type_;
  }

  double object::get_cords_x() const {
    return cords_x;
  }

  double object::get_cords_y() const {
    return cords_y;
  }

  double object::get_cords_z() const {
    return cords_z;
  }

  double object::get_radius() const {
    return radius;
  }

}  // namespace render
