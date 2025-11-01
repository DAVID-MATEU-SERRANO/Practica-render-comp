#include "../include/pov.hpp"
#include <cmath>
#define _USE_MATH_DEFINES

namespace render {

  ImageSize Pov::compute_image_size(int image_width, int aspect_ratio_w, int aspect_ratio_h) {
    double ratio     = static_cast<double>(aspect_ratio_h) / static_cast<double>(aspect_ratio_w);
    int image_height = static_cast<int>(std::floor(image_width * ratio));
    return ImageSize{image_width, image_height};
  }

}  // namespace render
