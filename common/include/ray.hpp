#ifndef RENDER_RAY_HPP
#define RENDER_RAY_HPP

#include "color.hpp"
#include "cylinder.hpp"
#include "point.hpp"
#include "scene.hpp"
#include "sphere.hpp"
#include "vector.hpp"

namespace render {

  using t_material = std::variant<Matte, Metal, Refractive>;

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

    void color_contribution(Color const & dark_color, Color const & light_color,
                            std::uint64_t seed);
    void matte_color_contribution(Matte const & matte, std::uint64_t seed);
    void background_color_contribution(Color const & dark_color, Color const & light_color);
    void metal_color_contribution(Metal const & metal, std::uint64_t seed);
    void refractive_color_contribution(Refractive const & refractive);

    bool test_sphere_intersections(Scene const & scene, double & closest_distance,
                                   Point & closest_point, Vector & closest_normal);
    bool test_cylinder_intersections(Scene const & scene, double & closest_distance,
                                     Point & closest_point, Vector & closest_normal);
    void find_closest_intersection(Scene const & scene);

  private:
    Point origin;
    Vector direction;
    Point point_intersection;
    Vector normal_vector;
    double intersection_distance = 0.0;
    t_material intersection_material;
    Color intersection_color = Color(1.0, 1.0, 1.0);
    Vector reflected_direction;
  };

}  // namespace render

#endif
