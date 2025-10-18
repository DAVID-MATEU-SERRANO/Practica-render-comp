#include "../include/metal.hpp"

namespace render {

  std::string Metal::get_name() const {
    return name;
  }

  double Metal::get_reflect1() const {
    return reflec1;
  }

  double Metal::get_reflect2() const {
    return reflec2;
  }

  double Metal::get_reflect3() const {
    return reflec3;
  }

  double Metal::get_difusion_factor() const {
    return difusion_factor;
  }

}  // namespace render
