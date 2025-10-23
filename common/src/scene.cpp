#include "../include/scene.hpp"
#include "../include/color.hpp"
#include "../include/point.hpp"
#include "../include/pov.hpp"
#include "../include/ray.hpp"
#include "../include/vector.hpp"
#include <cstdint>
#include <limits>
#include <random>

namespace render {

  Pov Scene::get_pov() const {
    return pov;
  }

  bool Scene::test_sphere_intersections(Ray & ray, double & closest_distance, Point & closest_point,
                                        Vector & closest_normal) {
    bool found_intersection = false;

    for (auto const & sphere : spheres) {
      if (ray.sphere_intersection(sphere) and ray.get_intersection_distance() >= 0) {
        // Update if the intersection is closer
        if (ray.get_intersection_distance() < closest_distance) {
          closest_distance   = ray.get_intersection_distance();
          closest_point      = ray.get_point_intersection();
          closest_normal     = ray.get_normal_vector();
          found_intersection = true;
        }
      }
    }
    return found_intersection;
  }

  bool Scene::test_cylinder_intersections(Ray & ray, double & closest_distance,
                                          Point & closest_point, Vector & closest_normal) {
    bool found_intersection = false;

    for (auto const & cylinder : cylinders) {
      if (ray.cylinder_side_intersection(cylinder) and ray.get_intersection_distance() >= 0) {
        // Update if the intersection is closer
        if (ray.get_intersection_distance() < closest_distance) {
          closest_distance   = ray.get_intersection_distance();
          closest_point      = ray.get_point_intersection();
          closest_normal     = ray.get_normal_vector();
          found_intersection = true;
        }
      }

      if (ray.cylinder_upper_base_intersection(cylinder) and ray.get_intersection_distance() >= 0) {
        // Update if the intersection is closer
        if (ray.get_intersection_distance() < closest_distance) {
          closest_distance   = ray.get_intersection_distance();
          closest_point      = ray.get_point_intersection();
          closest_normal     = ray.get_normal_vector();
          found_intersection = true;
        }
      }

      if (ray.cylinder_lower_base_intersection(cylinder) and ray.get_intersection_distance() >= 0) {
        // Update if the intersection is closer
        if (ray.get_intersection_distance() < closest_distance) {
          closest_distance   = ray.get_intersection_distance();
          closest_point      = ray.get_point_intersection();
          closest_normal     = ray.get_normal_vector();
          found_intersection = true;
        }
      }
    }
    return found_intersection;
  }

  void Scene::find_closest_intersection(Ray & ray) {
    double closest_distance = std::numeric_limits<double>::max();
    bool found_intersection = false;

    Point closest_point;
    Vector closest_normal;

    // Test intersections with all scene objects (spheres and cylinders)
    found_intersection =
        test_sphere_intersections(ray, closest_distance, closest_point, closest_normal) or
        found_intersection;
    found_intersection =
        test_cylinder_intersections(ray, closest_distance, closest_point, closest_normal) or
        found_intersection;

    if (found_intersection) {
      // Closest intersection data
      ray.set_intersection_distance(closest_distance);
      ray.set_point_intersection(closest_point);
      ray.set_normal_vector(closest_normal);
    } else {
      // Intersection not found
      ray.set_intersection_distance(-1.0);
    }
  }

  Pixel Scene::get_pixel_color(int f, int c) {
    std::mt19937_64 rng(rays_rng_seed);
    std::uniform_real_distribution<double> dist(-0.5, 0.5);
    Color pixel_color;
    Color accumulated_color(0.0, 0.0, 0.0);

    // TODO: ∆x and ∆y añadirlos como posibles atributos a la ventana de proyección.
    Vector const dx =
        pov.pw_horizontal_vector().dot(static_cast<double>(1.0 / pov.get_image_width()));
    Vector const dy =
        pov.pw_vertical_vector().dot(static_cast<double>(1.0 / pov.get_image_height()));

    for (int ray_counter = 0; ray_counter < samples_per_pixel; ++ray_counter) {
      double const rx = dist(rng);
      double const ry = dist(rng);
      Point const q   = pov.get_proyection_window()
                          .get_origin()
                          .add(dx.dot(static_cast<double>(c + rx)))
                          .add(dy.dot(static_cast<double>(f + ry)));

      Ray ray(pov.get_camera_position(), q.substract(pov.get_camera_position()),
              Color(1.0, 1.0, 1.0));
      for (int depth = 0; depth < max_depth; ++depth) {
        find_closest_intersection(ray);
        ray.color_contribution(background_dark_color, background_light_color, material_rng_seed);
        if (depth != max_depth - 1) {
          ray = Ray(ray.get_point_intersection(), ray.get_reflected_direction(),
                    ray.get_intersection_color());
        }
      }
      pixel_color       = ray.get_intersection_color().apply_gamma_correction(gamma);
      accumulated_color = accumulated_color.add(pixel_color);
    }
    accumulated_color = accumulated_color.multiply(1.0 / static_cast<double>(samples_per_pixel));
    return {static_cast<std::uint8_t>(255.0 * accumulated_color.get_r()),
            static_cast<std::uint8_t>(255.0 * accumulated_color.get_g()),
            static_cast<std::uint8_t>(255.0 * accumulated_color.get_b())};
  }

}  // namespace render
