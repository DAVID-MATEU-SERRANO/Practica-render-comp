#include "../include/pov.hpp"
#include "../include/point.hpp"
#include "../include/vector.hpp"
#include <cmath>
#include <numbers>

namespace render {

  // Getters
  Point Pov::get_camera_position() const {
    return camera_position;
  }

  Point Pov::get_camera_target() const {
    return camera_target;
  }

  Vector Pov::get_camera_north() const {
    return camera_north;
  }

  double Pov::get_field_of_view() const {
    return field_of_view;
  }

  int Pov::get_image_height() const {
    return image_size.image_height;
  }

  int Pov::get_image_width() const {
    return image_size.image_width;
  }

  Proyection_window Pov::get_proyection_window() const {
    return proyection_window;
  }

  // Ventana de proyección

  Vector Pov::pw_focal_vector() const {
    return camera_position.substract(camera_target);
  }

  double Pov::pw_focal_distance() const {
    return camera_position.substract(camera_target).magnitude();
  }

  double Pov::pw_height() const {
    return 2.0 * pw_focal_distance() * std::tan((field_of_view * std::numbers::pi / 180.0) / 2.0);
  }

  double Pov::pw_width() const {
    return pw_height() * (static_cast<double>(image_size.image_width) /
                          (static_cast<double>(image_size.image_height)));
  }

  Vector Pov::pw_director_vector_u() const {
    return (camera_north.cross(pw_focal_vector().normalized())).normalized();
  }

  Vector Pov::pw_director_vector_v() const {
    return pw_focal_vector().normalized().cross(pw_director_vector_u());
  }

  Vector Pov::pw_horizontal_vector() const {
    return pw_director_vector_u().dot(pw_width());
  }

  Vector Pov::pw_vertical_vector() const {
    return pw_director_vector_v().dot(-1.0).dot(pw_height());
  }

  Point Pov::pw_origin() const {
    return camera_position.substract(pw_focal_vector())
        .substract(pw_horizontal_vector().add(pw_vertical_vector()).dot(0.5))
        .substract(pw_horizontal_vector()
                       .dot(static_cast<double>(1.0 / image_size.image_width))
                       .add(pw_vertical_vector()
                                .dot(static_cast<double>(1.0 / image_size.image_height))
                                .dot(0.5)));
  }

}  // namespace render
