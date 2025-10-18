#include "../include/pov.hpp"

namespace render {

  Vector const & Pov::get_camera_position() const {
    return cam_pos;
  }

  Vector const & Pov::get_camera_target() const {
    return cam_tar;
  }

  Vector const & Pov::get_camera_north() const {
    return cam_north;
  }

  double Pov::get_field_of_view() const {
    return fov;
  }

}  // namespace render
