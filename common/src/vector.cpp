#include "vector.hpp"
#include <stdexcept>

namespace render {

  double Vector::magnitude() const {
    return std::sqrt(x * x + y * y + z * z);
  }

  Vector Vector::normalized() const {
    double mag = magnitude();
    if (mag == 0) {
      throw std::runtime_error("Cannot normalize zero vector");
    }
    return {x / mag, y / mag, z / mag};
  }

  Vector Vector::add(Vector const & other) const {
    return {x + other.x, y + other.y, z + other.z};
  }

  Vector Vector::substract(Vector const & other) const {
    return {x - other.x, y - other.y, z - other.z};
  }

  Vector Vector::dot(double scalar) const {
    return {x * scalar, y * scalar, z * scalar};
  }

  double Vector::dot(Vector const & other) const {
    return x * other.x + y * other.y + z * other.z;
  }

  Vector Vector::cross(Vector const & other) const {
    return {y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x};
  }

  Vector Vector::perpendicular_component(Vector const & other) const {
    return this->substract(other.dot(this->dot(other)));
  }

  double Vector::get_x() const {
    return x;
  }

  double Vector::get_y() const {
    return y;
  }

  double Vector::get_z() const {
    return z;
  }

}  // namespace render
