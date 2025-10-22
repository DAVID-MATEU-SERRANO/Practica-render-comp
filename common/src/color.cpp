#include "../include/color.hpp"
#include <stdexcept>

namespace render {

  int Color::get_r() const {
    if (r < 0 or r > 255) {
      throw std::runtime_error("Error: Invalid color parameters");
    }
    return r;
  }

  int Color::get_g() const {
    if (g < 0 or g > 255) {
      throw std::runtime_error("Error: Invalid color parameters");
    }
    return g;
  }

  int Color::get_b() const {
    if (b < 0 or b > 255) {
      throw std::runtime_error("Error: Invalid color parameters");
    }
    return b;
  }

}  // namespace render
