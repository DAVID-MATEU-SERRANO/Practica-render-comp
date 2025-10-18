#include "../include/ray.hpp"

namespace render {

  Vector const & Ray::get_origin() const {
    return origin;
  }

  Vector const & Ray::get_direction() const {
    return direction;
  }

}  // namespace render
