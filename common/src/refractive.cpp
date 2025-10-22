#include "../include/refractive.hpp"
#include <stdexcept>

namespace render {

  std::string Refractive::get_name() const {
    return name;
  }

  double Refractive::get_refraction_index() const {
    if (refraction_index < 1.0) {
      throw std::runtime_error("Error: Invalid refractive index");
    }
    return refraction_index;
  }

}  // namespace render
