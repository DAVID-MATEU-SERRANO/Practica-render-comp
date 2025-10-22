#ifndef RENDER_RAY_HPP
#define RENDER_RAY_HPP

#include "color.hpp"
#include "cylinder.hpp"
#include "point.hpp"
#include "sphere.hpp"
#include "vector.hpp"
#include <cstdint>

namespace render {

  using t_material = std::variant<Matte, Metal, Refractive>;

  class Ray {
  public:
    Ray(Point const & origin, Vector const & direction, Color intersection_color)
        : origin(origin), direction(direction), point_intersection(0.0, 0.0, 0.0),
          normal_vector(0.0, 0.0, 0.0), intersection_distance(),
          intersection_material(Matte("default", Color(1.0, 1.0, 1.0))),
          intersection_color(intersection_color), reflected_direction(0.0, 0.0, 0.0) { }

    [[nodiscard]] Point const & get_origin() const;
    [[nodiscard]] Vector const & get_direction() const;
    [[nodiscard]] Point const & get_point_intersection() const;
    [[nodiscard]] Vector const & get_normal_vector() const;
    [[nodiscard]] double get_intersection_distance() const;
    [[nodiscard]] t_material const & get_intersection_material() const;
    [[nodiscard]] Color const & get_intersection_color() const;
    [[nodiscard]] Vector const & get_reflected_direction() const;

    void set_point_intersection(Point const & point);
    void set_normal_vector(Vector const & normal);

    void set_intersection_distance(double distance);
    void set_intersection_material(t_material const & material);
    void set_intersection_color(Color const & color);
    void set_reflected_direction(Vector const & direction);

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

  private:
    Point origin;
    Vector direction;
    Point point_intersection;
    Vector normal_vector;
    double intersection_distance = -1.0;
    t_material intersection_material;
    Color intersection_color;
    Vector reflected_direction;
  };

}  // namespace render

#endif
