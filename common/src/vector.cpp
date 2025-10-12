#include "../include/vector.hpp"

namespace render {

  double Vector::magnitude() const {
    return std::sqrt(x * x + y * y + z * z);
  }

  Vector Vector::normalized() const {
    double mag = magnitude();

    if (mag == 0) {
      // Zero vector cannot be normalized (exception not implemented)
    }

    return {x / mag, y / mag, z / mag};
  }

  Vector Vector::add(Vector const & other) const {
    return {x + other.x, y + other.y, z + other.z};
  }

  Vector Vector::dot(double scalar) const {
    return {x * scalar, y * scalar, z * scalar};
  }

  Vector Vector::cross(Vector const & other) const {
    return {y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x};
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
