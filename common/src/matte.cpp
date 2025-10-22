#include "../include/matte.hpp"
#include <stdexcept>

namespace render {

  std::string Matte::get_name() const {
    return name;
  }

  Color Matte::get_reflectance() const {
    return reflectance;
  }

}  // namespace render
