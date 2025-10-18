#include "../include/refractive.hpp"

namespace render {

  std::string Refractive::get_name() const {
    return name;
  }

  double Refractive::get_refraction_index() const {
    return refraction_index;
  }

}  // namespace render
