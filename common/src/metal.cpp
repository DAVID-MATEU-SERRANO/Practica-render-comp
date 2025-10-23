#include "../include/metal.hpp"

namespace render {

  std::string Metal::get_name() const {
    return name;
  }

  Color Metal::get_reflectance() const {
    return reflectance;
  }

  double Metal::get_difusion_factor() const {
    return difusion_factor;
  }

}  // namespace render
