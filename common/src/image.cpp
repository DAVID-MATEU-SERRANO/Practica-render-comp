/*
#include "image.hpp"
#include <stdexcept>

namespace render {

  // Getters para los atributos
  std::tuple<int, int> Image::get_aspect_ratio() const {
    int x = std::get<0>(aspect_ratio);
    int y = std::get<1>(aspect_ratio);
    if (x <= 0 or y <= 0) {
      throw std::invalid_argument(
          "Invalid value: aspect ratio components must be positive integers.");
    }
    return aspect_ratio;
  }

  int Image::get_image_width() const {
    if (image_width <= 0) {
      throw std::invalid_argument("Invalid value: image width must be bigger than 0.");
    }
    return image_width;
  }

  int Image::get_image_height() const {
    if (image_height <= 0) {
      throw std::invalid_argument("Invalid value: image height must be bigger than 0.");
    }
    return image_height;
  }

  double Image::get_gamma() const {
    return gamma;
  }

  Pov const & Image::get_pov() const {
    return pov;
  }

  int Image::get_samples_per_pixel() const {
    if (samples_per_pixel <= 0) {
      throw std::invalid_argument("Invalid value: samples_per_pixel must be bigger than 0.");
    }
    return samples_per_pixel;
  }

  int Image::get_max_depth() const {
    if (max_depth <= 0) {
      throw std::invalid_argument("Invalid value: max_depth must be bigger than 0.");
    }
    return max_depth;
  }

  int Image::get_material_rng_seed() const {
    if (material_rng_seed <= 0) {
      throw std::invalid_argument("Invalid value: material_rng_seed must be bigger than 0.");
    }
    return material_rng_seed;
  }

  int Image::get_ray_rng_seed() const {
    if (ray_rng_seed <= 0) {
      throw std::invalid_argument("Invalid value: ray_rng_seed must be bigger than 0.");
    }
    return ray_rng_seed;
  }

  Vector const & Image::get_background_dark_color() const {
    if (background_dark_color.magnitude() < 0 or background_dark_color.magnitude() > 1) {
      throw std::invalid_argument(
          "Invalid value: background_dark_color components must be in the range [0, 1].");
    }
    return background_dark_color;
  }

  Vector const & Image::get_background_light_color() const {
    if (background_light_color.magnitude() < 0 or background_light_color.magnitude() > 1) {
      throw std::invalid_argument(
          "Invalid value: background_light_color components must be in the range [0, 1].");
    }
    return background_light_color;
  }

}  // namespace render
*/
