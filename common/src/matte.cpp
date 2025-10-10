#include "matte.hpp"

namespace render {

  std::string Matte::get_name() const {
    return name;
  }

  double Matte::get_reflect1() const {
    return reflec1;
  }

  double Matte::get_reflect2() const {
    return reflec2;
  }

  double Matte::get_reflect3() const {
    return reflec3;
  }

}  // namespace render
