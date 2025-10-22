#include "../include/matte.hpp"
#include <stdexcept>

namespace render {

  std::string Matte::get_name() const {
    return name;
  }

  double Matte::get_reflect1() const {
    if (reflec1 < 0.0 or reflec1 > 1.0) {
      throw std::runtime_error("Error: Invalid matte reflectivity parameters");
    }
    return reflec1;
  }

  double Matte::get_reflect2() const {
    if (reflec2 < 0.0 or reflec2 > 1.0) {
      throw std::runtime_error("Error: Invalid matte reflectivity parameters");
    }
    return reflec2;
  }

  double Matte::get_reflect3() const {
    if (reflec3 < 0.0 or reflec3 > 1.0) {
      throw std::runtime_error("Error: Invalid matte reflectivity parameters");
    }
    return reflec3;
  }

}  // namespace render
