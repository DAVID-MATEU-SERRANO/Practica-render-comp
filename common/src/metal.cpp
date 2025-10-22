#include "../include/metal.hpp"
#include <stdexcept>

namespace render {

  std::string Metal::get_name() const {
    return name;
  }

  double Metal::get_reflect1() const {
    if (reflec1 < 0.0 or reflec1 > 1.0) {
      throw std::runtime_error("Error: Invalid metal reflectivity parameters");
    }
    return reflec1;
  }

  double Metal::get_reflect2() const {
    if (reflec2 < 0.0 or reflec2 > 1.0) {
      throw std::runtime_error("Error: Invalid metal reflectivity parameters");
    }
    return reflec2;
  }

  double Metal::get_reflect3() const {
    if (reflec3 < 0.0 or reflec3 > 1.0) {
      throw std::runtime_error("Error: Invalid metal reflectivity parameters");
    }
    return reflec3;
  }

  double Metal::get_difusion_factor() const {
    return difusion_factor;
  }

}  // namespace render
