#include "ray.hpp"
#include "../include/vector.hpp"
#include "sphere.hpp"
#include <cmath>
#include <cstdlib>
#include <limits>
#include <random>

namespace render {

  Point const & Ray::get_origin() const {
    return origin;
  }

  Vector const & Ray::get_direction() const {
    return direction;
  }

  bool Ray::sphere_intersection(Sphere const & sphere) {
    double a = std::pow(direction.magnitude(), 2);
    double b = 2.0 * direction.dot(sphere.get_center().substract(origin));
    double c = std::pow(sphere.get_center().substract(origin).magnitude(), 2) -
               std::pow(sphere.get_radius(), 2);

    double discriminant = std::sqrt(b * b - 4 * a * c);
    if (discriminant < 0) {
      return false;
    }
    double t1 = (-b - discriminant) / (2 * a);
    double t2 = (-b + discriminant) / (2 * a);

    if (t1 < 0 and t2 < 0) {
      return false;
    }
    if (t1 >= 0 and t2 >= 0) {
      intersection_distance = std::min(t1, t2);
    } else if (t1 >= 0) {
      intersection_distance = t1;
    } else {
      intersection_distance = t2;
    }

    point_intersection = origin.add(direction.dot(intersection_distance));

    normal_vector = point_intersection.substract(sphere.get_center()).dot(1 / sphere.get_radius());
    // Ensure the normal vector points against the ray direction
    if (normal_vector.dot(direction) > 0) {
      normal_vector = normal_vector.dot(-1);
    }

    return true;
  }

  bool Ray::cylinder_side_intersection(Cylinder const & cylinder) {
    Vector rc = origin.substract(cylinder.get_center());
    double a  = std::pow(direction.perpendicular_component(cylinder.get_edge()).magnitude(), 2);
    double b  = 2.0 * (rc.perpendicular_component(cylinder.get_edge())
                          .dot(direction.perpendicular_component(cylinder.get_edge())));
    double c  = std::pow(rc.perpendicular_component(cylinder.get_edge()).magnitude(), 2) -
               std::pow(cylinder.get_radius(), 2);

    double discriminant = std::sqrt(b * b - 4 * a * c);
    if (discriminant < 0) {
      return false;
    }
    double t1 = (-b - discriminant) / (2 * a);
    double t2 = (-b + discriminant) / (2 * a);
    if (t1 < 0 and t2 < 0) {
      return false;
    }
    if (t1 >= 0 and t2 >= 0) {
      intersection_distance = std::min(t1, t2);
    } else if (t1 >= 0) {
      intersection_distance = t1;
    } else {
      intersection_distance = t2;
    }

    point_intersection = origin.add(direction.dot(intersection_distance));
    if (point_intersection.substract(cylinder.get_center()).dot(cylinder.get_edge()) >
        (cylinder.get_height() / 2))
    {
      return false;
    }

    normal_vector = point_intersection.substract(cylinder.get_center())
                        .perpendicular_component(cylinder.get_edge());
    // Ensure the normal vector points against the ray direction
    if (normal_vector.dot(direction) > 0) {
      normal_vector = normal_vector.dot(-1);
    }

    return true;
  }

  bool Ray::cylinder_upper_base_intersection(Cylinder const & cylinder) {
    Point p       = cylinder.get_center().add(cylinder.get_edge().dot(cylinder.get_height() / 2));
    normal_vector = cylinder.get_edge();
    Vector rp     = origin.substract(p);

    if (std::abs(direction.dot(normal_vector)) < 1e-8) {
      return false;
    }

    intersection_distance = rp.dot(normal_vector) / direction.dot(normal_vector);

    point_intersection = origin.add(direction.dot(intersection_distance));
    if (point_intersection.substract(p).magnitude() > cylinder.get_radius()) {
      return false;
    }

    // Ensure the normal vector points against the ray direction
    if (normal_vector.dot(direction) > 0) {
      normal_vector = normal_vector.dot(-1);
    }

    return true;
  }

  bool Ray::cylinder_lower_base_intersection(Cylinder const & cylinder) {
    Point p = cylinder.get_center().substract(cylinder.get_edge().dot(cylinder.get_height() / 2));
    normal_vector = cylinder.get_edge().dot(-1);
    Vector rp     = origin.substract(p);

    if (std::abs(direction.dot(normal_vector)) < 1e-8) {
      return false;
    }

    intersection_distance = rp.dot(normal_vector) / direction.dot(normal_vector);

    point_intersection = origin.add(direction.dot(intersection_distance));
    if (point_intersection.substract(p).magnitude() > cylinder.get_radius()) {
      return false;
    }

    // Ensure the normal vector points against the ray direction
    if (normal_vector.dot(direction) > 0) {
      normal_vector = normal_vector.dot(-1);
    }

    return true;
  }

  bool Ray::test_sphere_intersections(Scene const & scene, double & closest_distance,
                                      Point & closest_point, Vector & closest_normal) {
    bool found_intersection = false;

    for (auto const & sphere : scene.spheres) {
      if (sphere_intersection(sphere) and intersection_distance >= 0) {
        // Update if the intersection is closer
        if (intersection_distance < closest_distance) {
          closest_distance   = intersection_distance;
          closest_point      = point_intersection;
          closest_normal     = normal_vector;
          found_intersection = true;
        }
      }
    }
    return found_intersection;
  }

  bool Ray::test_cylinder_intersections(Scene const & scene, double & closest_distance,
                                        Point & closest_point, Vector & closest_normal) {
    bool found_intersection = false;

    for (auto const & cylinder : scene.cylinders) {
      if (cylinder_side_intersection(cylinder) and intersection_distance >= 0) {
        // Update if the intersection is closer
        if (intersection_distance < closest_distance) {
          closest_distance   = intersection_distance;
          closest_point      = point_intersection;
          closest_normal     = normal_vector;
          found_intersection = true;
        }
      }

      if (cylinder_upper_base_intersection(cylinder) and intersection_distance >= 0) {
        // Update if the intersection is closer
        if (intersection_distance < closest_distance) {
          closest_distance   = intersection_distance;
          closest_point      = point_intersection;
          closest_normal     = normal_vector;
          found_intersection = true;
        }
      }

      if (cylinder_lower_base_intersection(cylinder) and intersection_distance >= 0) {
        // Update if the intersection is closer
        if (intersection_distance < closest_distance) {
          closest_distance   = intersection_distance;
          closest_point      = point_intersection;
          closest_normal     = normal_vector;
          found_intersection = true;
        }
      }
    }
    return found_intersection;
  }

  void Ray::find_closest_intersection(Scene const & scene) {
    double closest_distance = std::numeric_limits<double>::max();
    bool found_intersection = false;

    Point closest_point;
    Vector closest_normal;

    // Test intersections with all scene objects (spheres and cylinders)
    found_intersection =
        test_sphere_intersections(scene, closest_distance, closest_point, closest_normal) or
        found_intersection;
    found_intersection =
        test_cylinder_intersections(scene, closest_distance, closest_point, closest_normal) or
        found_intersection;

    if (found_intersection) {
      // Closest intersection data
      intersection_distance = closest_distance;
      point_intersection    = closest_point;
      normal_vector         = closest_normal;
    } else {
      // Intersection not found
      intersection_distance = -1.0;
    }
  }

  void Ray::color_contribution(Color const & dark_color, Color const & light_color,
                               std::uint64_t seed) {
    // To be implemented: Calculate color contribution based on intersection and material
    if (intersection_distance == -1.0) {
      // No intersection, return background color
      background_color_contribution(dark_color, light_color);
      return;
    }
    if (std::holds_alternative<Matte>(intersection_material)) {
      matte_color_contribution(std::get<Matte>(intersection_material), seed);
    }

    if (std::holds_alternative<Metal>(intersection_material)) {
      metal_color_contribution(std::get<Metal>(intersection_material), seed);
    }

    if (std::holds_alternative<Refractive>(intersection_material)) {
      refractive_color_contribution(std::get<Refractive>(intersection_material));
    }
  }

  void Ray::background_color_contribution(Color const & dark_color, Color const & light_color) {
    double mix_factor = (direction.normalized().get_y() + 1.0) / 2.0;
    intersection_color =
        light_color.multiply(1.0 - mix_factor).add(dark_color.multiply(mix_factor));
  }

  void Ray::matte_color_contribution(Matte const & matte, std::uint64_t seed) {
    std::mt19937_64 rng(seed);
    std::uniform_real_distribution<double> dist(-1.0, 1.0);
    double random_value      = dist(rng);
    Vector reflection_vector = normal_vector.add_number(random_value);
    if (reflection_vector.get_x() < 1e-8 and
        reflection_vector.get_y() < 1e-8 and
        reflection_vector.get_z() < 1e-8)
    {
      reflected_direction = normal_vector;
    } else {
      reflected_direction = reflection_vector;
    }
    intersection_color = intersection_color.multiply(matte.get_reflectance());
  }

  void Ray::metal_color_contribution(Metal const & metal, std::uint64_t seed) {
    // To be implemented: Metal color contribution
    Vector initial_reflection =
        direction.substract(normal_vector.dot(2.0 * normal_vector.dot(direction)));

    std::mt19937_64 rng(seed);
    std::uniform_real_distribution<double> dist(-1.0 * metal.get_difusion_factor(),
                                                metal.get_difusion_factor());
    Vector diffusion_vector = Vector(dist(rng), dist(rng), dist(rng));
    reflected_direction     = initial_reflection.normalized().add(diffusion_vector);
    intersection_color      = intersection_color.multiply(metal.get_reflectance());
  }

  void Ray::refractive_color_contribution(Refractive const & refractive) {
    // To be implemented: Refractive color contribution
    double cos_0 = std::min(-normal_vector.normalized().dot(direction.normalized()), 1.0);
    double sin_0 = std::sqrt(1.0 - cos_0 * cos_0);

    double refraction_index_corrected = refractive.get_refraction_index();

    if (cos_0 < 0) {
      // Hacia dentro
      refraction_index_corrected = 1.0 / refraction_index_corrected;
    }

    if (refraction_index_corrected * sin_0 > 1.0) {
      // Total internal reflection
      reflected_direction =
          direction.substract(normal_vector.dot(2.0 * normal_vector.dot(direction)));

    } else {
      Vector u =
          direction.normalized().add(normal_vector.dot(cos_0)).dot(refraction_index_corrected);
      Vector v            = normal_vector.dot((-1.0) * std::sqrt(std::abs(1 - u.dot(u))));
      reflected_direction = u.add(v);
    }

    intersection_color = intersection_color.multiply(Color(1, 1, 1));
  }

}  // namespace render
