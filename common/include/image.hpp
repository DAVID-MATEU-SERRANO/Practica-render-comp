#ifndef RENDER_IMAGE_HPP
#define RENDER_IMAGE_HPP

#include "pov.hpp"
#include "vector.hpp"
#include <tuple>

namespace render {

  // Estructura de configuración con valores por defecto
  struct ImageConfig {
    std::tuple<int, int> aspect_ratio{16, 9};
    int image_width{1'920};
    double gamma{2.2};
    Pov pov;
    int samples_per_pixel{20};
    int max_depth{5};
    int material_rng_seed{13};
    int ray_rng_seed{19};
    Vector background_dark_color{0.25, 0.5, 1};
    Vector background_light_color{1, 1, 1};
  };

  class Image {
  public:
    // Constructor que recibe la configuración
    Image(ImageConfig const & cfg = ImageConfig())
        : aspect_ratio{cfg.aspect_ratio}, image_width{cfg.image_width}, gamma{cfg.gamma},
          pov{cfg.pov}, samples_per_pixel{cfg.samples_per_pixel}, max_depth{cfg.max_depth},
          material_rng_seed{cfg.material_rng_seed}, ray_rng_seed{cfg.ray_rng_seed},
          background_dark_color{cfg.background_dark_color},
          background_light_color{cfg.background_light_color} {
      // Calcular height automáticamente
      int x        = std::get<0>(aspect_ratio);
      int y        = std::get<1>(aspect_ratio);
      image_height = (image_width * y) / x;
    }

    // Getters para los atributos
    [[nodiscard]] std::tuple<int, int> get_aspect_ratio() const;
    [[nodiscard]] int get_image_width() const;
    [[nodiscard]] int get_image_height() const;
    [[nodiscard]] double get_gamma() const;
    [[nodiscard]] Pov const & get_pov() const;
    [[nodiscard]] int get_samples_per_pixel() const;
    [[nodiscard]] int get_max_depth() const;
    [[nodiscard]] int get_material_rng_seed() const;
    [[nodiscard]] int get_ray_rng_seed() const;
    [[nodiscard]] Vector const & get_background_dark_color() const;
    [[nodiscard]] Vector const & get_background_light_color() const;

  private:
    std::tuple<int, int> aspect_ratio;
    int image_width;
    int image_height;
    double gamma;
    Pov pov;
    int samples_per_pixel;
    int max_depth;
    int material_rng_seed;
    int ray_rng_seed;
    Vector background_dark_color;
    Vector background_light_color;
  };

}  // namespace render

#endif
