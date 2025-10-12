#ifndef RENDER_RAY_HPP
#define RENDER_RAY_HPP

#include "vector.hpp"

namespace render {

  class Ray {
  public:
    Ray(Vector const & origin, Vector const & direction) : origin(origin), direction(direction) { }

    [[nodiscard]] Vector const & get_origin() const;
    [[nodiscard]] Vector const & get_direction() const;

  private:
    Vector origin;
    Vector direction;
  };

}  // namespace render

#endif
