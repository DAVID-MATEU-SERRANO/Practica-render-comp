#include "../include/scene.hpp"
#include "../include/ray.hpp"
#include <random>

namespace render {

  Pixel Scene::get_pixel_color(int f, int c) const {
    std::mt19937_64 rng(rays_rng_seed);
    std::uniform_real_distribution<double> dist(-0.5, 0.5);

    // TODO: ∆x and ∆y añadirlos como posibles atributos a la ventana de proyección.
    Vector dx = pov.pw_horizontal_vector().dot(static_cast<double>(1.0 / pov.get_image_width()));
    Vector dy = pov.pw_vertical_vector().dot(static_cast<double>(1.0 / pov.get_image_height()));

    for (int ray_counter = 0; ray_counter < samples_per_pixel; ++ray_counter) {
      double rx = dist(rng);
      double ry = dist(rng);
      Point q   = pov.get_proyection_window()
                    .get_origin()
                    .add(dx.dot(static_cast<double>(c + rx)))
                    .add(dy.dot(static_cast<double>(f + ry)));
      Ray ray(camera_position, q.substract(camera_position));
      for (int depth = 0; depth < max_depth; ++depth) {
        ray.find_closest_intersection(*this);
        ray.color_contribution(background_dark_color, background_light_color,
                               rays_rng_seed + ray_counter * max_depth + depth);
      }
    }
  }

}  // namespace render
