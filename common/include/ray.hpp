#ifndef RENDER_RAY_HPP
#define RENDER_RAY_HPP

#include "cylinder.hpp"
#include "point.hpp"
#include "scene.hpp"
#include "sphere.hpp"
#include "vector.hpp"

namespace render {

  class Ray {
  public:
    Ray(Point const & origin, Vector const & direction)
        : origin(origin), direction(direction), point_intersection(0.0, 0.0, 0.0),
          normal_vector(0.0, 0.0, 0.0) { }

    [[nodiscard]] Point const & get_origin() const;
    [[nodiscard]] Vector const & get_direction() const;
    bool sphere_intersection(Sphere const & sphere);
    bool cylinder_side_intersection(Cylinder const & cylinder);
    bool cylinder_upper_base_intersection(Cylinder const & cylinder);
    bool cylinder_lower_base_intersection(Cylinder const & cylinder);
    void find_closest_intersection(Scene const & scene);

  private:
    Point origin;
    Vector direction;
    Point point_intersection;
    Vector normal_vector;
    double intersection_distance = 0.0;
  };

}  // namespace render

#endif
