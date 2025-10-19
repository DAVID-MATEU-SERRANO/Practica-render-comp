#include "../include/proyection_window.hpp"
#include "vector.hpp"
#include <cmath>

namespace render {

  // Getters
  Vector Proyection_window::get_focal_vector() const {
    return focal_vector;
  }

  double Proyection_window::get_focal_distance() const {
    return focal_distance;
  }

  double Proyection_window::get_height() const {
    return height;
  }

  double Proyection_window::get_width() const {
    return width;
  }

  Vector Proyection_window::get_horizontal_vector() const {
    return horizontal_vector;
  }

  Vector Proyection_window::get_vertical_vector() const {
    return vertical_vector;
  }

  Point Proyection_window::get_origin() const {
    return origin;
  }

};  // namespace render
