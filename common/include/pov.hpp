#ifndef RENDER_POV_HPP
#define RENDER_POV_HPP

#include "color.hpp"
#include "point.hpp"
#include "proyection_window.hpp"
#include "vector.hpp"
#include <cmath>
#include <cstdint>

namespace render {

  struct ImageSize {
    int image_width;
    int image_height;
  };

  // Clase usada para el punto de vista
  class Pov {
  public:
    Pov(Point camera_position, Point camera_target, Vector camera_north, double field_of_view,
        ImageSize image_size, std::uint64_t ray_seed)
        : camera_position{camera_position}, camera_target{camera_target},
          camera_north{camera_north}, field_of_view{field_of_view}, image_size{image_size},
          ray_seed{ray_seed}, proyection_window{Proyection_window(
                                  pw_focal_vector(), pw_focal_distance(), pw_height(), pw_width(),
                                  pw_horizontal_vector(), pw_vertical_vector(), pw_origin())},
          samples_per_pixel(), max_depth(), material_rng_seed(), rays_rng_seed(),
          background_dark_color(1.0, 1.0, 1.0), background_light_color(0.25, 0.5, 1) { }

    // Getters para los atributos
    [[nodiscard]] Point get_camera_position() const;
    [[nodiscard]] Point get_camera_target() const;
    [[nodiscard]] Vector get_camera_north() const;
    [[nodiscard]] double get_field_of_view() const;
    [[nodiscard]] int get_image_height() const;
    [[nodiscard]] int get_image_width() const;
    [[nodiscard]] std::uint64_t get_ray_seed() const;
    [[nodiscard]] Proyection_window get_proyection_window() const;

    // Calculos ventana proyección
    [[nodiscard]] Vector pw_focal_vector() const;
    [[nodiscard]] double pw_focal_distance() const;
    [[nodiscard]] double pw_height() const;
    [[nodiscard]] double pw_width() const;
    [[nodiscard]] Vector pw_director_vector_u() const;
    [[nodiscard]] Vector pw_director_vector_v() const;
    [[nodiscard]] Vector pw_horizontal_vector() const;
    [[nodiscard]] Vector pw_vertical_vector() const;
    [[nodiscard]] Point pw_origin() const;

  private:
    Point camera_position;
    Point camera_target;
    Vector camera_north;
    double field_of_view;
    ImageSize image_size;
    uint64_t ray_seed;
    Proyection_window proyection_window;
    // Otros datos
    int samples_per_pixel;         // Muestras por pixel
    int max_depth;                 // Profundidad maxima de rayos
    uint64_t material_rng_seed;    // Semilla para el generador de numeros aleatorios de materiales
    uint64_t rays_rng_seed;        // Semilla para el generador de numeros aleatorios de rayos
    Color background_dark_color;   // Color oscuro del fondo
    Color background_light_color;  // Color claro del fondo
  };

}  // namespace render

#endif
