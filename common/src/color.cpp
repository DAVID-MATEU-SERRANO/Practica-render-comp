#include "../include/color.hpp"
#include <cmath>

namespace render {

  Color Color::apply_gamma_correction(double gamma) const {
    return {std::pow(r, 1.0 / gamma), std::pow(g, 1.0 / gamma), std::pow(b, 1.0 / gamma)};
  }

}  // namespace render
