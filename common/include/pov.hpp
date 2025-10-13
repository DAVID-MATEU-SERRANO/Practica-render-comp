#ifndef RENDER_POV_HPP
#define RENDER_POV_HPP

#include "vector.hpp"

namespace render {

  // Clase usada para la imagen
  class Pov {
  public:
    Pov(Vector const & camera_position = Vector(0, 0, -10),
        Vector const & camera_target   = Vector(0, 0, 0),
        Vector const & camera_north = Vector(0, 1, 0), double field_of_view = 90.0)
        : cam_pos{camera_position}, cam_tar{camera_target}, cam_north{camera_north},
          fov{field_of_view} { }

    // Getters para los atributos
    [[nodiscard]] Vector const & get_camera_position() const;
    [[nodiscard]] Vector const & get_camera_target() const;
    [[nodiscard]] Vector const & get_camera_north() const;
    [[nodiscard]] double get_field_of_view() const;

  private:
    Vector cam_pos;
    Vector cam_tar;
    Vector cam_north;
    double fov;
  };

}  // namespace render

#endif
